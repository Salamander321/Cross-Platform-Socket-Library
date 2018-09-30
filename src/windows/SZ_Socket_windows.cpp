#include "../SZ_Socket.h"
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdint.h>
#include <iostream>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

const SZ_Connection SZ_MAX_CONNECTION = 0x7fffffff;
const SZ_Port SZ_DEFAULT_PORT = 8000;
const SZ_Port SZ_ANY_PORT = 0;
static bool sz_apiStarted = false;

SZ_API SZ_InitializeAPI()
{
	WSADATA wsaData;
	int errCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (errCode != 0)
		return SZ_API::SZ_INITIALIZATION_FAILED;
	sz_apiStarted = true;
	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_OpenServerSocket(SZ_IPVersion ipv, SZ_Address address, SZ_Port port, SZ_Protocol protocol, SZ_Connection connections, SZ_Socket* pSocket)
{
	if (!sz_apiStarted) return SZ_API::SZ_API_NOT_INITIALIZED;

	int errCode = 0;

	ADDRINFOA hints;
	ZeroMemory(&hints, sizeof(hints));
	if (ipv == SZ_IPVersion::SZ_IPv4)
		hints.ai_family = AF_INET;
	else
		hints.ai_flags = AF_INET6;

	hints.ai_flags = AI_PASSIVE;

	if (protocol == SZ_Protocol::SZ_TCP)
	{
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
	}
	else
	{
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;
	}

	char port_str[6];
	snprintf(port_str, 6 * sizeof(char), "%u", port);
	ADDRINFOA* result = null;
	errCode = GetAddrInfoA(address, port_str, &hints, &result);
	if (errCode != 0)
		return SZ_API::SZ_SOCKET_OPEN_FAILED;

	SOCKET wSocket = INVALID_SOCKET;
	wSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (wSocket == INVALID_SOCKET)
		return SZ_API::SZ_SOCKET_OPEN_FAILED;

	errCode = bind(wSocket, result->ai_addr, (int)result->ai_addrlen);
	if (errCode == SOCKET_ERROR)
	{
		FreeAddrInfoA(result);
		closesocket(wSocket);
		return SZ_API::SZ_SOCKET_BIND_FAILED;
	}
	strcpy_s((char*)pSocket->address, result->ai_addrlen * sizeof(char), result->ai_addr->sa_data);
	FreeAddrInfoA(result);

	errCode = listen(wSocket, connections);
	if (errCode == SOCKET_ERROR) 
	{
		closesocket(wSocket);
		return SZ_API::SZ_SOCKET_LISTEN_FAILED;
	}

	pSocket->connections = connections;
	pSocket->port = port;
	pSocket->protocol = protocol;
	pSocket->handle = new SOCKET;
	*(SOCKET*)pSocket->handle = wSocket;

	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_AcceptClient(const SZ_Socket& server, SZ_Socket* client)
{
	sockaddr addr;
	int addr_len = sizeof(sockaddr);
	SOCKET clientSocket = accept(*(SOCKET*)server.handle, &addr, &addr_len);
	if (clientSocket == INVALID_SOCKET)
		return SZ_API::SZ_SOCKET_ACCEPT_FAILED;

	strcpy_s((char*)client->address, addr_len * sizeof(char), addr.sa_data);
	client->protocol = server.protocol;
	client->handle = new SOCKET;
	*(SOCKET*)client->handle = clientSocket;

	return SZ_API::SZ_SUCCESS;
}

SZ_API SZ_OpenClientSocket(SZ_Address address, SZ_Port port, SZ_Protocol protocol, SZ_Socket* pSocket)
{
	if (!sz_apiStarted) return SZ_API::SZ_API_NOT_INITIALIZED;

	int errCode = 0;

	ADDRINFOA hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;

	if (protocol == SZ_Protocol::SZ_TCP)
	{
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
	}
	else
	{
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;
	}

	char port_str[6];
	snprintf(port_str, 6 * sizeof(char), "%u", port);
	ADDRINFOA* result = null, * ptr = null;
	errCode = GetAddrInfoA(address, port_str, &hints, &result);
	if (errCode != 0)
		return SZ_API::SZ_SOCKET_OPEN_FAILED;

	SOCKET wSocket = INVALID_SOCKET;
	for (ptr = result; ptr != null; ptr = ptr->ai_next) 
	{
		wSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (wSocket == INVALID_SOCKET)
			return SZ_API::SZ_SOCKET_OPEN_FAILED;

		errCode = connect(wSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (errCode == SOCKET_ERROR) 
		{
			closesocket(wSocket);
			wSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	strcpy_s((char*)pSocket->address, result->ai_addrlen * sizeof(char), result->ai_addr->sa_data);
	FreeAddrInfoA(result);
	if (wSocket == INVALID_SOCKET) 
		return SZ_API::SZ_SOCKET_OPEN_FAILED;

	pSocket->port = port;
	pSocket->protocol = protocol;
	pSocket->handle = new SOCKET;
	*(SOCKET*)pSocket->handle = wSocket;

	return SZ_API::SZ_SUCCESS;
}

void SZ_Receive(const SZ_Socket& client, char* buffer, int size, SZ_Callback callback)
{
	int result = 1;
	unsigned int callbackResult = 1;
	SOCKET clientSocket = *(SOCKET*)client.handle;
	while(result > 0 && callbackResult != 0)
	{
		result = recv(clientSocket, buffer, size, 0);
		if (result > 0)
			callbackResult = callback(SZ_BYTE_RECEIVED, &client, buffer, result);
		else if (result == 0)
			callbackResult = callback(SZ_CLOSING_CONNECTION, &client, null, 0);
		else 
		{
			switch (result)
			{
			case WSANOTINITIALISED:
				callbackResult = callback(SZ_NOT_INITIALIZED, &client, null, 0);
				break;

			case WSAENETDOWN:
				callbackResult = callback(SZ_NETWORK_DOWN, &client, null, 0);
				break;

			case WSAEFAULT:
				callbackResult = callback(SZ_BUFFER_ERROR, &client, null, 0);
				break;

			case WSAENOTCONN:
				callbackResult = callback(SZ_SOCKET_NOT_CONNECTED, &client, null, 0);
				break;

			case WSAEINTR:
				callbackResult = callback(SZ_BLOCKED, &client, null, 0);
				break;

			case WSAEINPROGRESS:
				callbackResult = callback(SZ_IN_PROGRESS, &client, null, 0);
				break;

			case WSAENETRESET:
				callbackResult = callback(SZ_BROKEN_CONNECTION, &client, null, 0);
				break;

			case WSAENOTSOCK:
				callbackResult = callback(SZ_NOT_SOCKET, &client, null, 0);
				break;

			case WSAEOPNOTSUPP:
				callbackResult = callback(SZ_UNSUPPORTED_DATA, &client, null, 0);
				break;

			case WSAESHUTDOWN:
				callbackResult = callback(SZ_SOCKET_SHUTDOWNED, &client, null, 0);
				break;

			case WSAEWOULDBLOCK:
				callbackResult = callback(SZ_BLOCKED, &client, null, 0);
				break;

			case WSAEMSGSIZE:
				callbackResult = callback(SZ_MSG_LARGER_THAN_BUFFER, &client, null, 0);
				break;

			case WSAEINVAL:
				callbackResult = callback(SZ_ERROR_FLAG, &client, null, 0);
				break;

			case WSAECONNABORTED:
				callbackResult = callback(SZ_TIMED_OUT, &client, null, 0);
				break;

			case WSAETIMEDOUT:
				callbackResult = callback(SZ_TIMED_OUT, &client, null, 0);
				break;

			case WSAECONNRESET:
				callbackResult = callback(SZ_CONNECTION_RESET, &client, null, 0);
				break;

				default:
				callbackResult = callback(SZ_FAILED_TO_RECEIVE, &client, null, 0);
				break;
			}
		}
	}

	shutdown(clientSocket, SD_SEND);
}

SZ_Message SZ_Send(const SZ_Socket& client, const char* buffer, int size, int* sent)
{
	int result = send(*(SOCKET*)client.handle, buffer, size, 0);
	if (result > 0)
	{
		if (sent != null)
			*sent = result;
		return SZ_BYTE_SENT;
	}
	switch (result)
	{
	case WSANOTINITIALISED: return SZ_NOT_INITIALIZED;
	case WSAENETDOWN: return SZ_NETWORK_DOWN;
	case WSAEFAULT: return SZ_BUFFER_ERROR;
	case WSAENOTCONN: return SZ_SOCKET_NOT_CONNECTED;
	case WSAEINTR: return SZ_BLOCKED;
	case WSAEINPROGRESS: return SZ_IN_PROGRESS;
	case WSAENETRESET: return SZ_BROKEN_CONNECTION;
	case WSAENOTSOCK: return SZ_NOT_SOCKET;
	case WSAEOPNOTSUPP: return SZ_UNSUPPORTED_DATA;
	case WSAESHUTDOWN: return SZ_SOCKET_SHUTDOWNED;
	case WSAEWOULDBLOCK: return SZ_BLOCKED;
	case WSAEMSGSIZE: return SZ_MSG_LARGER_THAN_BUFFER;
	case WSAEINVAL: return SZ_ERROR_FLAG;
	case WSAECONNABORTED: return SZ_TIMED_OUT;
	case WSAETIMEDOUT: return SZ_TIMED_OUT;
	case WSAECONNRESET: return SZ_CONNECTION_RESET;
	default: return SZ_FAILED_TO_SEND;
	}
	return SZ_FAILED_TO_SEND;
}

void SZ_SocketStopRcv(SZ_Socket* skt)
{
	if (skt->handle == null) return;
	shutdown(*(SOCKET*)skt->handle, SD_RECEIVE);
}

void SZ_SocketStopSend(SZ_Socket* skt)
{
	if (skt->handle == null) return;
	shutdown(*(SOCKET*)skt->handle, SD_SEND);
}

void SZ_CloseSocket(SZ_Socket* skt)
{
	if (skt->handle == null) return;
	closesocket(*(SOCKET*)skt->handle);
	delete skt->handle;
	skt->handle = null;
}

void SZ_ShutdownAPI()
{
	if (!sz_apiStarted) return;
	sz_apiStarted = false;
	WSACleanup();
}
