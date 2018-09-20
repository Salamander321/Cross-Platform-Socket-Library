#include "../SZ_Socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//NOTE (Zero): Your API code for linux should go here

//static bool sz_apiStarted = false;

SZ_API SZ_InitializeAPI()
{

	//sz_apiStarted = true;
	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_OpenSocket(SZ_Address, SZ_Port, SZ_Protocal, SZ_Socket**)
{

	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_BindSocket(SZ_Socket*)
{

	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_ListenFromSocket(SZ_Socket*)
{

	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_ConnectToSocket(SZ_Socket*)
{

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