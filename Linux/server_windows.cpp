// server


#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <WS2tcpip.h>

#define port "2000"
#define null 0

#include <cstdio>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSADATA wsaData;
	int errCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (errCode != 0)
	{
		printf("Error! Winshock Initialization failed, err code %d", errCode);
		return errCode;
	}

	PADDRINFOA result = null, ptr = null;
	ADDRINFOA hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	errCode = GetAddrInfoA(null, port, &hints, &result);
	if (errCode != 0)
	{
		printf("GetAddrInfo failed, err code: %d", errCode);
		WSACleanup();
		return errCode;
	}

	SOCKET listenSocket = INVALID_SOCKET;
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (listenSocket == INVALID_SOCKET)
	{
		errCode = WSAGetLastError();
		printf("Error socket creation, err code: %d", errCode);
		WSACleanup();
		return errCode;
	}

	errCode = bind(listenSocket, result->ai_addr, result->ai_addrlen);
	if (errCode == SOCKET_ERROR)
	{
		printf("Socket Binding Error, err code: %d", errCode);
		FreeAddrInfoA(result);
		closesocket(listenSocket);
		WSACleanup();
		return errCode;
	}
	FreeAddrInfoA(result);

	errCode = listen(listenSocket, SOMAXCONN);
	if (errCode == SOCKET_ERROR)
	{
		printf("Listen failed, err code: %d", errCode);
		closesocket(listenSocket);
		WSACleanup();
		return errCode;
	}

	SOCKET clientSocket = INVALID_SOCKET;
	clientSocket = accept(listenSocket, null, null);
	if (clientSocket == INVALID_SOCKET)
	{
		errCode = WSAGetLastError();
		printf("Accept error, err code: %d", errCode);
		closesocket(listenSocket);
		WSACleanup();
		return errCode;
	}

	const size_t bufferSize = 512; // hard-coded, may need to change

	const char* testMessage = "this is a test message";
	char recivedMsg[bufferSize];

	do
	{
		errCode = recv(clientSocket, recivedMsg, bufferSize, 0);
		if (errCode > 0)
		{
			printf("Bytes received: %d", errCode);
			int send_res = send(clientSocket, recivedMsg, bufferSize, 0);
			if (send_res == SOCKET_ERROR)
			{
				printf("Sending failed, err code: %d", send_res);
				closesocket(clientSocket);
				WSACleanup();
				return send_res;
			}
			printf("Bytes sent: %d", send_res);
		}
		else if (errCode == 0)
			printf("Connection closing");
		else
		{
			printf("Receive failed, err code: %d", errCode);
			closesocket(clientSocket);
			WSACleanup();
			return errCode;
		}
	} while (errCode > 0);
	closesocket(clientSocket);

	closesocket(listenSocket);
	WSACleanup();

	return 0;
}