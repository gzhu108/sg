#include "TcpSocket.h"
#include "Exception.h"

using namespace sg::microreactor;


TcpSocket::TcpSocket()
{
}

TcpSocket::TcpSocket(SOCKET socket)
{
    if (Attach(socket))
    {
        GetSocketName();
        GetPeerName();
    }
}

TcpSocket::~TcpSocket()
{
}

bool TcpSocket::Listen(const std::string& hostName, uint16_t port)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    // Close previous socket
    Detach();

    if (!CreateSocketFromAddress(hostName, port, SOCK_STREAM, IPPROTO_TCP, true))
    {
        return false;
    }

    // Set SO_REUSEADDR on the socket to true (1)
    int32_t optval = 1;
    SetSockOpt(SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));

    // Set SO_LINGER on the socket to false (0):
    linger lingerval = { 0 };
    SetSockOpt(SOL_SOCKET, SO_LINGER, (char*)&lingerval, sizeof(lingerval));

    // Setup the TCP listening socket
    int32_t result = bind(mSocket, mAddrInfo->ai_addr, (int32_t)mAddrInfo->ai_addrlen);
    if (result == SOCKET_ERROR)
    {
        Detach();
        int32_t error = GetSocketError();
        THROW(SocketException, error, hostName, port);
    }

    // Save the host name and port number
    if (mAddrInfo->ai_canonname != nullptr)
    {
        HostName.set(mAddrInfo->ai_canonname);
        HostPort.set(port);
    }
    else
    {
        GetSocketName();
    }

    // Listen to the socket
    result = listen(mSocket, SOMAXCONN);
    if (result == SOCKET_ERROR)
    {
        Detach();
        int32_t error = GetSocketError();
        THROW(SocketException, error, hostName, port);
    }

    return true;
}

std::shared_ptr<TcpSocket> TcpSocket::Accept(const std::chrono::milliseconds& timeout)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    std::unique_ptr<timeval> tv;
    if (timeout.count() != 0)
    {
        std::chrono::seconds timeoutSeconds = std::chrono::duration_cast<std::chrono::seconds>(timeout);
        std::chrono::milliseconds timeoutRemainder = timeout - std::chrono::duration_cast<std::chrono::milliseconds>(timeoutSeconds);
        std::chrono::microseconds timeoutMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(timeoutRemainder);
        
        tv.reset(new timeval());
        tv->tv_sec = (decltype(tv->tv_sec))timeoutSeconds.count();
        tv->tv_usec = (decltype(tv->tv_usec))timeoutMicroseconds.count();
    }

    // Select the server socket for checking incoming connection.
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(mSocket, &readfds);
    
    // Wait until timeout for incoming connection, tv = NULL for blocking operation.
    int32_t result = select((int32_t)mSocket + 1, &readfds, nullptr, nullptr, tv.get());
    if (result == SOCKET_ERROR)
    {
        // select error
        int32_t error = GetSocketError();
        THROW(SocketException, error, HostName.cref(), HostPort.cref());
    }
    else if (result > 0)
    {
        // Accept a client socket
        SOCKET socket = accept(mSocket, nullptr, nullptr);
        if (socket == INVALID_SOCKET)
        {
            return nullptr;
        }
        
        std::shared_ptr<TcpSocket> clientSocket = std::make_shared<TcpSocket>(socket);
        return clientSocket;
    }

    return nullptr;
}

bool TcpSocket::Connect(const std::string& address, uint16_t port, const std::chrono::milliseconds& timeout)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    // Close previous socket
    Detach();

    if (!CreateSocketFromAddress(address, port, SOCK_STREAM, IPPROTO_TCP, false))
    {
        return false;
    }

    // Blocking connections
    int32_t result = connect(mSocket, mAddrInfo->ai_addr, (int32_t)mAddrInfo->ai_addrlen);
    if (result == SOCKET_ERROR)
    {
        Detach();
        int32_t error = GetSocketError();
        THROW(SocketException, error, address, port);
    }
    else
    {
        // Get the host name
        if (mAddrInfo->ai_canonname != nullptr)
        {
            HostName.set(mAddrInfo->ai_canonname);
            HostPort.set(port);
        }
        else
        {
            GetSocketName();
        }

        // Get the connection name
        if (GetPeerName())
        {
            LOG("TCP [%s]:%d -> [%s]:%d", HostName->c_str(), HostPort.cref(), PeerName->c_str(), PeerPort.cref());
            mConnected();
        }

        return true;
    }

    return false;
}

bool TcpSocket::SetKeepalive(TcpSocket& socket, int32_t keepalive, int32_t keepaliveProbes, int32_t keepaliveIdleTime, int32_t keepaliveProbeInterval)
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
