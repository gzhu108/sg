#include "TcpSocket.h"
#include "Exception.h"

using namespace microreactor;


TcpSocket::TcpSocket()
{
}

TcpSocket::TcpSocket(SOCKET socket)
{
    if (Attach(socket))
    {
        GetSocketAddress();
        GetPeerAddress();
    }
}

TcpSocket::~TcpSocket()
{
}

bool TcpSocket::Listen(const std::string& hostAddress, uint16_t port)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    // Close previous socket
    Detach();

    if (!CreateSocketFromAddress(hostAddress, port, SOCK_STREAM, IPPROTO_TCP, true))
    {
        return false;
    }

    // Set SO_REUSEADDR on the socket to true (1)
    int optval = 1;
    SetSockOpt(SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));

    // Set SO_LINGER on the socket to false (0):
    linger lingerval = { 0 };
    SetSockOpt(SOL_SOCKET, SO_LINGER, (char*)&lingerval, sizeof(lingerval));

    // Setup the TCP listening socket
    int result = bind(mSocket, mAddrInfo->ai_addr, (int)mAddrInfo->ai_addrlen);
    if (result == SOCKET_ERROR)
    {
        Detach();
        int error = GetSocketError();
        THROW(SocketException, error, hostAddress, port);
    }

    // Save the host name and port number
    GetSocketAddress();

    // Listen to the socket
    result = listen(mSocket, SOMAXCONN);
    if (result == SOCKET_ERROR)
    {
        Detach();
        int error = GetSocketError();
        THROW(SocketException, error, hostAddress, port);
    }

    // Set the socket into non-blocking mode
    SetNonblocking(true);

    return true;
}

std::shared_ptr<TcpSocket> TcpSocket::Accept()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    // Accept a client socket
    SOCKET socket = accept(mSocket, nullptr, nullptr);
    if (socket == INVALID_SOCKET)
    {
        return nullptr;
    }
        
    std::shared_ptr<TcpSocket> clientSocket = std::make_shared<TcpSocket>(socket);

    // Set the socket into non-blocking mode
    clientSocket->SetNonblocking(true);

    return clientSocket;
}

bool TcpSocket::Connect(const std::string& address, uint16_t port)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    // Close previous socket
    Detach();

    if (!CreateSocketFromAddress(address, port, SOCK_STREAM, IPPROTO_TCP, false))
    {
        return false;
    }

    // Blocking connections
    int result = connect(mSocket, mAddrInfo->ai_addr, (int)mAddrInfo->ai_addrlen);
    if (result == SOCKET_ERROR)
    {
        Detach();
        int error = GetSocketError();
        THROW(SocketException, error, address, port);
    }
    else
    {
        // Get the host name
        GetSocketAddress();

        // Get the connection name
        if (GetPeerAddress())
        {
            LOG("TCP [%s]:%d -> [%s]:%d", HostAddress->c_str(), HostPort.cref(), PeerAddress->c_str(), PeerPort.cref());
            mConnected();
        }

        // Set the socket into non-blocking mode
        SetNonblocking(true);

        return true;
    }

    return false;
}

bool TcpSocket::SetKeepalive(TcpSocket& socket, int keepalive, int keepaliveProbes, int keepaliveIdleTime, int keepaliveProbeInterval)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

#ifdef _MSC_VER
    UNREFERENCED_PARAMETER(keepaliveProbes);

    // Enable keepalive on the socket.
    DWORD dwKeepalive = keepalive;
    if (setsockopt(socket.mSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&dwKeepalive, sizeof(dwKeepalive)) == SOCKET_ERROR)
    {
        return false;
    }
    
    DWORD bytesReturned = 0;
    
    tcp_keepalive tcpKeepalive;
    tcpKeepalive.onoff = keepalive;
    tcpKeepalive.keepalivetime = keepaliveIdleTime;
    tcpKeepalive.keepaliveinterval = keepaliveProbeInterval;
    
    return WSAIoctl(
        socket.mSocket,
        SIO_KEEPALIVE_VALS,             // dwIoControlCode
        (LPVOID)&tcpKeepalive,          // pointer to tcp_keepalive struct 
        (DWORD)sizeof(tcpKeepalive),    // length of input buffer 
        NULL,                           // output buffer
        0,                              // size of output buffer
        (LPDWORD)&bytesReturned,        // number of bytes returned
        NULL,                           // OVERLAPPED structure
        NULL                           // completion routine
    ) != SOCKET_ERROR;

#else

#if SO_KEEPALIVE
    // Enable keepalive on the socket.
    if (setsockopt(socket.mSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepalive, sizeof(keepalive)) == SOCKET_ERROR)
    {
        return false;
    }
#endif
    
#if TCP_KEEPCNT
    // The maximum number of keepalive probes TCP should send before dropping the connection
    if (setsockopt(socket.mSocket, IPPROTO_TCP, TCP_KEEPCNT, (char*)&keepaliveProbes, sizeof(keepaliveProbes)) == SOCKET_ERROR)
    {
        return false;
    }
#endif
    
#if TCP_KEEPIDLE
    // The time (in seconds) the connection needs to remain idle before TCP starts sending keepalive probes
    if (setsockopt(socket.mSocket, IPPROTO_TCP, TCP_KEEPIDLE, (char*)&keepaliveIdleTime, sizeof(keepaliveIdleTime)) == SOCKET_ERROR)
    {
        return false;
    }
#endif

#if TCP_KEEPINTVL
    // The time (in seconds) between individual keepalive probes
    if (setsockopt(socket.mSocket, IPPROTO_TCP, TCP_KEEPINTVL, (char*)&keepaliveProbeInterval, sizeof(keepaliveProbeInterval)) == SOCKET_ERROR)
    {
        return false;
    }
#endif

    return true;

#endif
}
