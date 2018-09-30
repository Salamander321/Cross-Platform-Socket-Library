#include "../../SZ_Socket.h"
#include <cstring>
#include <iostream>

// Errors are not maintained properly and only accepting single server

// Error checking, break if error -- deferencing nullptr will cause break
#ifdef _WIN32
#define SZAPI(x) if(x!=SZ_SUCCESS) __debugbreak()
#else
#define SZAPI(x) if(x!=SZ_SUCCESS) (nullptr)
#endif

// Receive callback
unsigned int OnMessageReceive(SZ_Message msg, const SZ_Socket* client, char* buffer, int received)
{
	if (msg == SZ_BYTE_RECEIVED)
	{
		printf("Message sucessfully revieved, total characters received: %d\n", received);
		printf("Message: %s\n", buffer);
		return 0; // return 1 to continue receiving
	}
	else
	{
		printf("Some error occurred while receiving\n");
		return 0; // return 1 to continue receiving
	}
}

int main()
{
	SZAPI(SZ_InitializeAPI());

	SZ_Address address = "127.0.0.1"; // ip-address

	SZ_Socket client;
	SZAPI(SZ_OpenClientSocket(address, 2000, SZ_TCP, &client));
	char msg[234];
	printf("Enter a message to send to server ");
	scanf("%s",msg);
	int recvd;
	SZ_Message res = SZ_Send(client, msg, (int)strlen(msg), &recvd);
	if (res == SZ_BYTE_SENT)
		printf("\"%s\" sucessfully sent!\n", msg);
	else
		printf("Some error occurred and message could not be sent\n");

	char* buffer = new char[500];
	SZ_Receive(client, buffer, 500, OnMessageReceive);
	delete[] buffer;

	SZ_CloseSocket(&client);

	SZ_ShutdownAPI();

	std::cin.get();
	return 0;
}
