#include "../../SZ_Socket.h"
#include <iostream>
#include <cstring>
/*This is a simple http client that send HTTP GET request to Google Server
* and receive response */

// Receive callback

unsigned int OnMessageReceive(SZ_Message msg, const SZ_Socket* client, char* buffer, int received)
{
	if (msg == SZ_BYTE_RECEIVED)
	{
		printf("Response from server is:-  %s\n", buffer);
		return 0; // return 1 to continue receiving
	}
	else
	{
		printf("Some error occurred while receiving from Server\n");
		return 0; // return 1 to continue receiving
	}
	}


int main()
{
	SZ_InitializeAPI();
	SZ_Socket Client;
	
	//IP address of google.com
	SZ_Address address ="172.217.17.142";
	
	//Most of HTTP servers work on 80 port no.
	SZ_OpenClientSocket(address,80,SZ_TCP,&Client);
    //characters for sending http request to server
	char request[] = "GET /HTTP/1.1\r\n\r\n";
   	
   	int echk;
	SZ_Message chk = SZ_Send(Client,request,(int)strlen(request),&echk);
	if (chk == SZ_BYTE_SENT)
		printf("\"%s\" sucessfully sent!\n" );
	else
		printf("Some error occurred and message could not be sent\n");
	
	char* response = new char[4096];
	SZ_Receive(Client,response,4096,OnMessageReceive);
	delete[] response;
	

	SZ_CloseSocket(&Client);
	
	std::cin.get();

	return 0;

}
