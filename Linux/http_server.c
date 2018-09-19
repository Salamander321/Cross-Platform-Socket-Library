#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{	//Creating file structure that HTTP web server is about to display
	FILE *read_data;
	read_data = fopen("index.html","r");
	char response_data[1024];
	fgets(response_data,1024,read_data);
	//creating http header for confirmation of communication
	char http_header[2048] = "HTTP/1.1 200 OK\r\n\n";
	strcat(http_header,response_data);
	//create a server socket
	int server_socket;
	server_socket = socket(AF_INET,SOCK_STREAM,0);
	//define server structure
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8000);
	server_address.sin_addr.s_addr = INADDR_ANY;//to specifiy you can use inet_addr(<server_address>) function
	//bind address to socket
	bind(server_socket,(struct sockaddr *) &server_address, sizeof(server_address));
	listen(server_socket,6);
	
	//Initialising Client Socket
	int client_socket;
	while (1)
	{
		client_socket = accept(server_socket,NULL,NULL);
		send(client_socket,http_header,sizeof(http_header),0);
		close(client_socket);
	}

	return 0;
}
