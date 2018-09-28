#include "../SZ_Socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <netdb.h>

const SZ_Connection SZ_MAX_CONNECTION = 0x7fffffff;

SZ_API SZ_InitializeAPI()
{
	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_OpenServerSocket(SZ_Port port, SZ_Protocol protocol, SZ_Connection connections, SZ_Socket* pSocket)
{
	int errCode = 0;

	addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;

	if (protocol == SZ_Protocol::SZ_TCP)
		hints.ai_socktype = SOCK_STREAM;
	else
		hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = 0;

	char port_str[6];
	snprintf(port_str, 6 * sizeof(char), "%u", port);
	addrinfo* result = null;
	errCode = getaddrinfo(null, port_str, &hints, &result);
	if (errCode != 0)
		return SZ_API::SZ_SOCKET_OPEN_FAILED;

	int lSocket = -1;
	lSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (lSocket == -1)
		return SZ_API::SZ_SOCKET_OPEN_FAILED;

	errCode = bind(lSocket, result->ai_addr, result->ai_addrlen);
	if (errCode != 0)
	{
		freeaddrinfo(result);
		close(lSocket);
		return SZ_API::SZ_SOCKET_BIND_FAILED;
	}
	strcpy((char*)pSocket->address, result->ai_addr->sa_data);
	freeaddrinfo(result);

	errCode = listen(lSocket, connections);
	if (errCode != 0)
	{
		close(lSocket);
		return SZ_API::SZ_SOCKET_LISTEN_FAILED;
	}

	pSocket->connections = connections;
	pSocket->port = port;
	pSocket->protocol = protocol;
	pSocket->handle = new int;
	*(int*)pSocket->handle = lSocket;

	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_AcceptClient(const SZ_Socket& server, SZ_Socket* client)
{
	int clientSocket = -1;
	sockaddr addr;
	unsigned int addr_len;
	clientSocket = accept(*(int*)server.handle, &addr, &addr_len);
	if (clientSocket == -1)
		return SZ_API::SZ_SOCKET_ACCEPT_FAILED;

	strcpy(client->address, ((sockaddr*)&addr)->sa_data);
	client->protocol = server.protocol;
	client->handle = new int;
	*(int*)client->handle = clientSocket;

	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_OpenClientSocket(SZ_Address address, SZ_Port port, SZ_Protocol protocol, SZ_Socket* pSocket)
{
	int errCode = -1;

	addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	
	if (protocol == SZ_Protocol::SZ_TCP)
		hints.ai_socktype = SOCK_STREAM;
	else
		hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = 0;

	char port_str[6];
	snprintf(port_str, 6, "%u", port);
	addrinfo* result = null, *ptr = null;
	errCode = getaddrinfo(address, port_str, &hints, &result);
	if (errCode != 0)
		return SZ_API::SZ_SOCKET_OPEN_FAILED;

	int lSocket = -1;
	for (ptr = result; ptr != null; ptr = ptr->ai_next)
	{
		lSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (lSocket == -1)
			return SZ_API::SZ_SOCKET_OPEN_FAILED;

		errCode = connect(lSocket, ptr->ai_addr, ptr->ai_addrlen);
		if (errCode != 0)
		{
			close(lSocket);
			lSocket = -1;
			continue;
		}
		break;
	}

	snprintf(pSocket->address, result->ai_addrlen * sizeof(char), "%s", result->ai_addr->sa_data);
	freeaddrinfo(result);
	if (lSocket == -1)
		return SZ_API::SZ_SOCKET_OPEN_FAILED;

	pSocket->port = port;
	pSocket->protocol = protocol;
	pSocket->handle = new int;
	*(int*)pSocket->handle = lSocket;

	return SZ_API::SZ_SUCCESS;
}

void SZ_Receive(const SZ_Socket& client, char* buffer, int size, SZ_Callback callback)
{
	ssize_t result = 1;
	unsigned int callbackResult = 1;
	int clientSocket = *(int*)client.handle;
	while (result != 0 || callbackResult > 0)
	{
		result = recv(clientSocket, buffer, size, 0);
		if (result > 0)
			callbackResult = callback(SZ_BYTE_RECEIVED, buffer, result);
		else if (result == 0)
			callbackResult = callback(SZ_CLOSING_CONNECTION, null, 0);
		else
		{
			switch (errno)
			{
			case EAGAIN:
				callbackResult = callback(SZ_TIMED_OUT, null, 0);
				break;

			case EBADF:
				callbackResult = callback(SZ_NOT_SOCKET, null, 0);
				break;

			case ECONNREFUSED:
				callbackResult = callback(SZ_CONNECTION_REFUSED, null, 0);
				break;

			case EFAULT:
				callbackResult = callback(SZ_BUFFER_ERROR, null, 0);
				break;

			case EINTR:
				callbackResult = callback(SZ_IN_PROGRESS, null, 0);
				break;

			case EINVAL:
				callbackResult = callback(SZ_UNSUPPORTED_DATA, null, 0);
				break;

			case ENOMEM:
				callbackResult = callback(SZ_MSG_LARGER_THAN_BUFFER, null, 0);
				break;

			case ENOTCONN:
				callbackResult = callback(SZ_SOCKET_NOT_CONNECTED, null, 0);
				break;

			case ENOTSOCK:
				callbackResult = callback(SZ_NOT_SOCKET, null, 0);
				break;

			default:
				callbackResult = callback(SZ_FAILED_TO_RECEIVE, null, 0);
				break;
			}
		}
	}
}

SZ_Message SZ_Send(const SZ_Socket& client, char* buffer, int size, int* sent)
{
	int result = send(*(int*)client.handle, buffer, size, 0);
	if (result != -1)
	{
		if (sent != null)
			*sent = result;
		return SZ_BYTE_SENT;
	}

	switch (errno)
	{
	case EACCES: return SZ_FAILED_TO_SEND;
	case EAGAIN: return SZ_TIMED_OUT;
	case EBADF: return SZ_NOT_SOCKET;
	case ECONNREFUSED: return SZ_CONNECTION_REFUSED;
	case EFAULT: return SZ_BUFFER_ERROR;
	case EINTR: return SZ_IN_PROGRESS;
	case EINVAL: return SZ_UNSUPPORTED_DATA;
	case ENOMEM: return SZ_MSG_LARGER_THAN_BUFFER;
	case ENOTCONN: return SZ_SOCKET_NOT_CONNECTED;
	case ENOTSOCK: return SZ_NOT_SOCKET;
	case EPIPE: return SZ_SOCKET_SHUTDOWNED;
	default: return SZ_FAILED_TO_SEND;
	}
	return SZ_FAILED_TO_SEND;
}

void SZ_SocketStopRcv(SZ_Socket* skt)
{
	if (skt->handle == null) return;
	shutdown(*(int*)skt->handle, SHUT_RD);
}

void SZ_SocketStopSend(SZ_Socket* skt)
{
	if (skt->handle == null) return;
	shutdown(*(int*)skt->handle, SHUT_WR);
}

void SZ_CloseSocket(SZ_Socket* skt)
{
	if (skt->handle == null) return;
	close(*(int*)skt->handle);
	delete skt->handle;
	skt->handle = null;
}

void SZ_ShutdownAPI()
{
}
