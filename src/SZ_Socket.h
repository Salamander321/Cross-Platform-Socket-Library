// Data Types
// Port value ranges from 0 to 65535 so unsigned short is chosen
typedef unsigned short SZ_Port;     //HACK (Zero): Range of unsigned short 0 to 65,535 on all platforms?
typedef char SZ_Address[20];        //HACK (Zero): Size of the address only upto 20?
typedef void* SZ_Handle;

enum SZ_Protocal
{
    SZ_TCP, SZ_UDP
    /*NOTE (Zero): Other protocal support may be added if required */
};

struct SZ_Socket
{
	void* handle;
    SZ_Address address;
    SZ_Port port;
    SZ_Protocal protocal;
    /*NOTE (Zero): Other properties can be added if required */
};

// API Error Management
enum SZ_API
{
    SZ_SUCCESS = 0,
    SZ_INITIALIZATION_FAILED,
    SZ_INVALID_SOCKED_PROVIDED,
    SZ_API_NOT_INITIALIZED,
    SZ_SHUTDOWN_FAILED,
    SZ_SOCKET_OPEN_FAILED,
    SZ_SOCKET_BIND_FAILED,
    SZ_SOCKET_LISTEN_FAILED,
    SZ_SOCKET_CONNECT_FAILED,
    SZ_SOCKET_UNABLE_TO_CLOSE,
    SZ_SOCKET_COULD_NOT_SEND,
    SZ_SOCKED_COULD_NOT_RECV,
    /*NOTE (Zero): Others will be added as required */
};


// Available Functions //

// Initializes Socket API
SZ_API SZ_InitializeAPI();

// Create Socket 
SZ_API SZ_OpenSocket(SZ_Address, SZ_Port, SZ_Protocal, SZ_Socket**);

// Bind Socket
SZ_API SZ_BindSocket(SZ_Socket*);

// Listen to socket
SZ_API SZ_ListenFromSocket(SZ_Socket*);

// Connect to the server
SZ_API SZ_ConnectToSocket(SZ_Socket*);

// Wait until a new client comes up
SZ_Socket SZ_AcceptSocket(SZ_Socket*);

// Send message to socket
SZ_API SZ_SendTo(SZ_Socket*, char* msg, unsigned int size);

// Receive message from socket
SZ_API SZ_ReceiveFrom(SZ_Socket*, char* msg, unsigned int size);

// Close Socket
SZ_API SZ_CloseSocket(SZ_Socket*);

// Shutdown all API things
void SZ_ShutdownAPI();