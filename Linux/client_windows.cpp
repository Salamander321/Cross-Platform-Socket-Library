// client


#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <WS2tcpip.h>

#define port "2000"

#include <cstdio>

//#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSADATA wsaData;
	int errCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (errCode != 0)
	{
		printf("Error! Winshock Initialization failed, err code: %d", errCode);
		return errCode;
	}


	PADDRINFOA result, ptr;
	ADDRINFOA hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	errCode = GetAddrInfoA("192.168.1.105", port, &hints, &result);
	if (errCode != 0)
	{
		printf("Error! Getting Address failed, err code: %d", errCode);
		WSACleanup();
		return errCode;
	}

	SOCKET connectSocket = INVALID_SOCKET;


	ptr = result;
	connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (connectSocket == INVALID_SOCKET)
	{
		errCode = WSAGetLastError();
		printf("Error! Connecting to server failed, err code: %d", errCode);
		FreeAddrInfo(result);
		WSACleanup();
		return errCode;
	}

	errCode = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (errCode == SOCKET_ERROR) {
		closesocket(connectSocket);
		connectSocket = INVALID_SOCKET;
	}
	// TODO (zero) : check for other existing address returned by GetAddrInfoA

	FreeAddrInfo(result);

	if (connectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to the server");
		WSACleanup();
		return -1;
	}

	const size_t bufferSize = 512; // hard-coded, may need to change

	const char* testMessage = "this is a test message";
	char recivedMsg[bufferSize];
	
	errCode = send(connectSocket, testMessage, int(strlen(testMessage)), 0);
	if (errCode == SOCKET_ERROR)
	{
		printf("Msh sending failed, err code: %d", errCode);
		closesocket(connectSocket);
		WSACleanup();
		return errCode;
	}

	printf("Bytes sent: %d", errCode);
	errCode = shutdown(connectSocket, SD_SEND);
	if (errCode == SOCKET_ERROR) {
		errCode = WSAGetLastError();
		printf("shutdown failed: %d\n", errCode);
		closesocket(connectSocket);
		WSACleanup();
		return errCode;
	}

	do 
	{
		errCode = recv(connectSocket, recivedMsg, bufferSize, 0);
		if (errCode > 0)
			printf("Bytes received : %s", recivedMsg);
		else if (errCode == 0)
			printf("Connection lost");
		else
			printf("Receive data err, err code : %d", WSAGetLastError());
	} while (errCode > 0);


	closesocket(connectSocket);
	WSACleanup();
	return 0;
}