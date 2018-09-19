#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
	char server_message[256] = "You have reached the server!";
	//create the server socket
	int server_socket;
	server_socket = socket(AF_INET,SOCK_STREAM,0);
	//define the server address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8002);
	server_address.sin_addr.s_addr = INADDR_ANY;//inet_addr("127.0.0.1");
	printf("Server address is defined\n");
	//bind the socket to our specified IP and port
	bind(server_socket,(struct sockaddr*) &server_address,sizeof(server_address));
	listen(server_socket,6);
	printf("Address is binded and port is opened for listen\n");
	int client_socket;
	client_socket = accept(server_socket,NULL,NULL);
	printf("client is accepted\n");
	//send message
	send(client_socket ,server_message, sizeof(server_message),0);
	printf("Message is send to client\n");
	//receive message from client
	char client_response[256];
	recv(client_socket, &client_response, sizeof(client_response),0);
	//print client response
	printf("Message is received from client:- %s",client_response);
	
	//close the socket
	close(server_socket);
	return 0;

}
