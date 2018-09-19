#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
	char *address;
	address = argv[1];
	//creating a socket
	int client_socket;
	client_socket = socket(AF_INET,SOCK_STREAM,0);
	//defining server structure
	struct sockaddr_in remote_address;
	remote_address.sin_family = AF_INET;
	remote_address.sin_port = htons(80);
	//assign socket address given by user to remote
	remote_address.sin_addr.s_addr = inet_addr(address);

	//connect to server
	connect(client_socket, (struct sockaddr *) &remote_address, sizeof(remote_address));
	
	char request[] = "GET / HTTP1.1 \r\n\r\n"; //creating a HTTP request that we are going to send to server 
	char response[4096];
	
	//send a http request
	send(client_socket,request,sizeof(request),0);

	// receive http response
	recv(client_socket,&response,sizeof(response),0);
	printf("Response from the server: %s\n",response);
	close(client_socket);

	return 0;	




}
