#include "../../SZ_Socket.h"
#include <cstring>
#include <iostream>

/* This is just a simple HTTP server that displays index.html file when requested by http client.
 *  To test this just run this program and type localhost:8000 or 127.0.0.1:8000 at your browser
 *  since by default we have assign to IP of local host.
 */


int main()
{	
    
    //API has been Initialised
	SZ_API errmsg;

	errmsg = SZ_InitializeAPI();
	if (errmsg != SZ_SUCCESS)
		printf("Error during initialization");

	SZ_Address address = "127.0.0.2";
	SZ_Socket Server;
	//FILE pointer for html file we want to send to HTTP client
	FILE *read_data;
	fopen_s(&read_data, "index.html","r");
	char response_data[1024];
	fgets(response_data,1024,read_data);
	
	//HTTP header for HTTP client to verify transcation 
	char http_header[2048] = "HTTP/1.1 200 OK\r\n\n";
	strcat_s(http_header,response_data);
	
	errmsg = SZ_OpenServerSocket(address,8000,SZ_TCP,SZ_MAX_CONNECTION,&Server);
	if (errmsg != SZ_SUCCESS)
		printf("Error, could not open server socket");
	
	SZ_Socket Client;
	
	int eChk;
	do
	{
	
	SZ_API(SZ_AcceptClient(Server, &Client));
	SZ_Send(Client,http_header,sizeof(http_header),&eChk);

	
	}while (1);
	
	SZ_CloseSocket(&Server);
	SZ_CloseSocket(&Client);
	
	return 0;
	

}
