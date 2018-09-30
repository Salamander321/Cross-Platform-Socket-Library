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
		printf("Replying with \"Bye...bye...\"\n");
		const char* reply = "Bye...bye...";
		auto res = client->Reply(*client, reply, (int)strlen(reply), null); // null because we don't want number of bytes sent
		if (res == SZ_BYTE_SENT)
			printf("\"%s\" sucessfully sent!\n", reply);
		else
			printf("Some error occurred and message could not be sent\n");
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
    SZ_Address address = "127.0.0.1";
	SZ_Socket server;
	SZAPI(SZ_OpenServerSocket(address,2000, SZ_TCP, SZ_MAX_CONNECTION, &server));

	printf("Waiting for clients to show up...\n");
   // printf("Server address is %s\n",server.address);
	SZ_Socket client;
	SZAPI(SZ_AcceptClient(server, &client));

	char* buffer = new char[500];
	SZ_Receive(client, buffer, 500, OnMessageReceive);
	delete[] buffer;

	// Both client and server socket need to be closed
	SZ_CloseSocket(&client);
	SZ_CloseSocket(&server);

	SZ_ShutdownAPI();

	std::cin.get();
	return 0;
}
