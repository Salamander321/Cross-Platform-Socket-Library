#include "../SZ_Socket.h"
#include <cstdio>

int main()

{

SZ_Protocal Server_Protocal;

SZ_Socket Server_Socket;

Server_Socket.no_connection = 6;

char address[] = "127.0.0.1";//while passing direct message , it says ISO C++ forbids converting a string constant to ‘char*’ [-Wwrite-strings] 

//Initialisation of Socket
SZ_OpenSocket(address,8002,Server_Protocal,&Server_Socket);

//Binding Socket to its specific IP
SZ_BindSocket(&Server_Socket);

//Listen for connection from client
SZ_ListenFromSocket(&Server_Socket);

char server_message[256] = "You have reached the server!";

//Accepting Connection from client
SZ_Socket Client_Socket;
Client_Socket = SZ_AcceptSocket(&Server_Socket);
printf("client is accepted\n");

//Sending message to client
SZ_SendTo(&Client_Socket,server_message,sizeof(server_message));
printf("Message is send to client\n");

//Receiving message from client
char client_response[256];
SZ_ReceiveFrom(&Client_Socket,client_response,sizeof(client_response));
printf("Message is received from client:- %s",client_response);

//closing socket
SZ_CloseSocket(&Server_Socket);

return 0;

}
