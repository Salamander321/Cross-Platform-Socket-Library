#include "../SZ_Socket.h"
#include <cstdio>


int main() {
	//Initialisation of Socket
	SZ_Protocal Client_Protocal;
	SZ_Socket Client_Socket;
	char address[] = "127.0.0.1";
	SZ_OpenSocket(address,8002,Client_Protocal,&Client_Socket);
	
	//connecting to server
	SZ_ConnectToSocket(&Client_Socket);
	
	//receive data from server
	char server_response[256];
	SZ_ReceiveFrom(&Client_Socket,server_response,sizeof(server_response));
	printf("Data is received from server:%s",server_response);
	
	//send message to server.
	char client_message[256] = "Connected to Client by almighty pranav";
	SZ_SendTo(&Client_Socket,client_message,sizeof(client_message));
	printf("\nData is send to server\n");
	
	//close the socket
	SZ_CloseSocket(&Client_Socket);
	
	return 0;

}


