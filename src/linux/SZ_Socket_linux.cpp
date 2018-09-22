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
    Socket->SZ_Socket_address.sin_addr.s_addr = inet_addr(Socket->address);//INADDR_ANY;
    //opening socket
    Socket->handle = socket(AF_INET,SOCK_STREAM,Socket->protocal);
	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_BindSocket(SZ_Socket* Socket)
{

    int error = bind(Socket->handle,(struct sockaddr*) &Socket->SZ_Socket_address,sizeof(Socket->SZ_Socket_address));
        
    
	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_ListenFromSocket(SZ_Socket* Socket)
{
    listen(Socket->handle,Socket->no_connection);
	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_ConnectToSocket(SZ_Socket* Socket)
{
    int status = connect(Socket->handle,(struct sockaddr*) &Socket->SZ_Socket_address, sizeof(Socket->SZ_Socket_address));
	return SZ_API::SZ_SUCCESS;
}

SZ_Socket SZ_AcceptSocket(SZ_Socket* Socket)
{
       
	SZ_Socket resultSocket;
	resultSocket.handle = accept(Socket->handle,NULL,NULL);
	return resultSocket;
}

SZ_API SZ_SendTo(SZ_Socket* Socket, char* msg, unsigned int size)
{
    send(Socket->handle,msg,size,0);
	return SZ_API::SZ_SUCCESS;

}
SZ_API SZ_ReceiveFrom(SZ_Socket* Socket, char msg[], unsigned int size)
{
    char temp_message[256];
    recv(Socket->handle,&temp_message,sizeof(temp_message),0);
	strcpy(msg,temp_message);
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
