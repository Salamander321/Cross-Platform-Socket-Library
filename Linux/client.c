#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
	int network_socket; //creating network socket
	network_socket = socket(AF_INET,SOCK_STREAM,0);
	//specify an address fo the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8002);
	server_address.sin_addr.s_addr =INADDR_ANY; //inet_addr("127.0.0.1"); //real server address we are going to connect
	int connection_status=connect(network_socket,(struct sockaddr*) &server_address,sizeof(server_address));
	printf("Server Address is Specified\n");
	//check for connection
	if (connection_status == -1)
	{	printf("There was an error making connection to server\n\n");
	}
	
	printf("Connection is checked\n");
	//receive data from server
	char server_response[256];
	recv(network_socket, &server_response,sizeof(server_response),0);
	
	//print out the server response
	printf("Data is received from server:%s",server_response);
	//send message to server.
	char client_message[256] = "Connected to Client";
	send(network_socket,client_message,sizeof(client_message),0);
	printf("\nData is send to server\n");
	//close the socket
	close(network_socket);

	return 0;
}

