#include "../SZ_Socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>

//NOTE (Zero): Your API code for linux should go here
//(Salamander): No error handle has been made.
//static bool sz_apiStarted = false;

SZ_API SZ_InitializeAPI()
{

	//sz_apiStarted = true;
	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_OpenSocket(SZ_Address address, SZ_Port port, SZ_Protocal protocal, SZ_Socket* Socket)
{
    //Initialisation 
    strcpy(Socket->address,address);
    Socket->port = port;
    Socket->protocal = protocal;
    Socket->SZ_Socket_address.sin_port = htons(Socket->port);
    Socket->SZ_Socket_address.sin_family = AF_INET;
    Socket->SZ_Socket_address.sin_addr.s_addr = INADDR_ANY;//inet_addr(Socket->address);
    //opening socket
    Socket->handle = socket(AF_INET,SOCK_STREAM,Socket->protocal);
    //printf("Initialisation Completed and Value of handle is %d\n",Socket->handle);
    //printf("handle mama");
	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_BindSocket(SZ_Socket* Socket)
{

    int error = bind(Socket->handle,(struct sockaddr*) &Socket->SZ_Socket_address,sizeof(Socket->SZ_Socket_address));
    
    if (error < 0)
    {
    printf("There was an binding error");
    }
    
	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_ListenFromSocket(SZ_Socket* Socket)
{
    listen(Socket->handle,Socket->no_connection);
    struct sockaddr_in sin;
socklen_t len = sizeof(sin);
if (getsockname(Socket->handle,(struct sockaddr*) &Socket->SZ_Socket_address, &len) == -1)
    perror("getsockname");
else
    printf("port number %d\n", ntohs(sin.sin_port));
	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_ConnectToSocket(SZ_Socket* Socket)
{
    int status = connect(Socket->handle,(struct sockaddr*) &Socket->SZ_Socket_address, sizeof(Socket->SZ_Socket_address));
	return SZ_API::SZ_SUCCESS;
}

SZ_Socket SZ_AcceptSocket(SZ_Socket*)
{

	SZ_Socket resultSocket;

	return resultSocket;
}

SZ_API SZ_SendTo(SZ_Socket*, char* msg, unsigned int size)
{

	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_ReceiveFrom(SZ_Socket*, char* msg, unsigned int size)
{

	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_CloseSocket(SZ_Socket*)
{

	return SZ_API::SZ_SUCCESS;
}

void SZ_ShutdownAPI()
{
	//if(!sz_apiStarted) return;

	//sz_apiStarted = fasle;
}
