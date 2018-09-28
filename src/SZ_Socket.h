/*
	- Socket moZilla library for crossplatform (Windows and Linux) socket and networking
	- Authors Salamander(https://github.com/Salamander321) and Zero(https://github.com/00Zero0)
	- This software is available as open domain
*/

// Messages returned during sending and receiving process 
enum SZ_Message
{
	SZ_BYTE_RECEIVED = 0,
	SZ_BYTE_SENT,
	SZ_CLOSING_CONNECTION,
	SZ_FAILED_TO_RECEIVE,
	SZ_FAILED_TO_SEND,
	SZ_CONNECTION_REFUSED,
	SZ_NOT_INITIALIZED,
	SZ_NETWORK_DOWN,
	SZ_BUFFER_ERROR,
	SZ_SOCKET_NOT_CONNECTED,
	SZ_BLOCKED,
	SZ_IN_PROGRESS,
	SZ_BROKEN_CONNECTION,
	SZ_NOT_SOCKET,
	SZ_UNSUPPORTED_DATA,
	SZ_SOCKET_SHUTDOWNED,
	SZ_MSG_LARGER_THAN_BUFFER,
	SZ_ERROR_FLAG,
	SZ_TIMED_OUT,
	SZ_CONNECTION_RESET
};

// Port value ranges from 0 to 65535 so unsigned short is chosen
typedef unsigned short SZ_Port;
// Address max character can be upto 14 for IPV4, currently IPV6 not supported
typedef char SZ_Address[14];
// Internal pointer to platform specific handle to socket
typedef void* SZ_Handle;
// Number of connections supported by server, for max connection set this to SZ_MAX_CONNECTION
typedef int SZ_Connection;
// Call back when message is received
typedef unsigned int(*SZ_Callback)(SZ_Message, char*, int);

// Maximum connections supported in a single server
extern const SZ_Connection SZ_MAX_CONNECTION;

// Connection to any random port, setting this will cause the struct SZ_Socket to have port of 0
extern const SZ_Port SZ_ANY_PORT;

// it's just preference
#define null 0

// Only supports TCP and UDP protocol
enum SZ_Protocol
{
    SZ_TCP, SZ_UDP
};

/*
	- Socket structure for exposing API
	- The structure must not be filled by user and should only be filled by calling API functions
	- The members should be used read only
	- Server doesn't receive `port` information, so the value will be garbage for the struct received by server on connection
	- Client doesn't use `connections`, so the value will be garbage
*/
struct SZ_Socket
{
    SZ_Handle handle;
    SZ_Address address;
    SZ_Port port;
    SZ_Protocol protocol;
	SZ_Connection connections;
};

// Return values for all of the API calls, except in some cases when the function returns void
enum SZ_API
{
    SZ_SUCCESS = 0,
    SZ_INITIALIZATION_FAILED,
    SZ_API_NOT_INITIALIZED,
    SZ_SOCKET_OPEN_FAILED,
    SZ_SOCKET_BIND_FAILED,
    SZ_SOCKET_LISTEN_FAILED,
    SZ_SOCKET_CONNECT_FAILED,
	SZ_SOCKET_ACCEPT_FAILED
};


// Available Functions //

/*
	- Initializes the SZ_Sozket API
	- In Windows this function must be call to initialize required socket libraries
	- Other class won't work unless you call this functions in Windows
	- In linux, you may or may not call this function
	- If using as a crossplatform, call this function before doing anything
*/
SZ_API SZ_InitializeAPI();

// To start a Server
SZ_API SZ_OpenServerSocket(SZ_Port, SZ_Protocol, SZ_Connection, SZ_Socket*);

// Server accpets clients, wait until a client appairs
SZ_API SZ_AcceptClient(const SZ_Socket& server, SZ_Socket* client);

// To connect to a Server
SZ_API SZ_OpenClientSocket(SZ_Address, SZ_Port, SZ_Protocol, SZ_Socket*);

/*
	- Used to receive message
	- When in server, specify the socket of the client you want to receive message from
	- When in client, specify the socket of the client you want to receive message to
	- This functions keeps on receiving messages until the socket is closed
	- To stop further receiving message, the `callback` should return 0, to continue receiving 1
	- Callback is send with the status message, message received and number of character in message
	- If the status message is other than `SZ_BYTE_RECEIVED`, then the receiving failed and message is `null` and characters number is 0
*/
void SZ_Receive(const SZ_Socket& client, char* buffer, int size, SZ_Callback callback);

/*
	- Used to send message
	- When in server, specify the socket of the client you want to send message to
	- When in client, specify the socket of the client that you want to send message from
	- Returns `SZ_BYTE_SENT` is the process was a success
	- `sent` returns the number of characters sent, if `null` is specified, it is not used
*/
SZ_Message SZ_Send(const SZ_Socket& client, char* buffer, int size, int* sent);

// No further reception will be accepted from the socket after this function is called
void SZ_SocketStopRcv(SZ_Socket*);

// No further data can be sent from the socket after this function is called
void SZ_SocketStopSend(SZ_Socket*);

// No further reception and sending can occur from the socket after this function is called
void SZ_CloseSocket(SZ_Socket*);

// Shutdown all networking API calls, should be called after work has been completed
void SZ_ShutdownAPI();
