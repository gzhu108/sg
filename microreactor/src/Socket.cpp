#include "Socket.h"
#include "Exception.h"
#include "Endianness.h"
#include "NetworkUtility.h"

#ifndef IPv4LOOPBACK
#define IPv4LOOPBACK 0x100007f
#endif

using namespace sg::microreactor;


Socket::Socket()
    : mSocket(INVALID_SOCKET)
    , mAddrInfo(nullptr)
{
#ifdef _MSC_VER
    memset(&mSocketInfo, 0, sizeof(mSocketInfo));
    
    // Initialize Winsock
    int result = WSAStartup(MAKEWORD(2, 2), &mSocketInfo);
    mInitialized = (result == 0);
#endif
}

Socket::~Socket()
{
    Detach();

#ifdef _MSC_VER
    if (mInitialized)
    {
        WSACleanup();
        mInitialized = false;
    }
#endif
}

void Socket::SetAddrInfo(std::shared_ptr<addrinfo> addrInfo)
{
    mAddrInfo = addrInfo;
}

bool Socket::SetReceiveBufferSize(uint32_t sizeInBytes)
{
    return SetSockOpt(SOL_SOCKET, SO_RCVBUF, (char*)&sizeInBytes, sizeof(uint32_t)) != SOCKET_ERROR;
}

bool Socket::SetSendBufferSize(uint32_t sizeInBytes)
{
    return SetSockOpt(SOL_SOCKET, SO_SNDBUF, (char*)&sizeInBytes, sizeof(uint32_t)) != SOCKET_ERROR;
}

bool Socket::SetReceiveTimeout(const std::chrono::milliseconds& timeout)
{
#ifdef _MSC_VER
    uint32_t receiveTimeout = (uint32_t)timeout.count();
#else
    std::chrono::seconds timeoutSeconds = std::chrono::duration_cast<std::chrono::seconds>(timeout);
    std::chrono::milliseconds timeoutRemainder = timeout - std::chrono::duration_cast<std::chrono::milliseconds>(timeoutSeconds);
    std::chrono::microseconds timeoutMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(timeoutRemainder);

    timeval receiveTimeout;
    receiveTimeout.tv_sec = (decltype(receiveTimeout.tv_sec))timeoutSeconds.count();
    receiveTimeout.tv_usec = (decltype(receiveTimeout.tv_usec))timeoutMicroseconds.count();
#endif

    int result = SetSockOpt(SOL_SOCKET, SO_RCVTIMEO, (char*)&receiveTimeout, sizeof(receiveTimeout));
    if (result == SOCKET_ERROR)
    {
        LOG("Failed to set receive timeout (%d)", GetSocketError());
        return false;
    }

    return true;
}

bool Socket::SetSendTimeout(const std::chrono::milliseconds& timeout)
{
#ifdef _MSC_VER
    uint32_t sendTimeout = (uint32_t)timeout.count();
#else
    std::chrono::seconds timeoutSeconds = std::chrono::duration_cast<std::chrono::seconds>(timeout);
    std::chrono::milliseconds timeoutRemainder = timeout - std::chrono::duration_cast<std::chrono::milliseconds>(timeoutSeconds);
    std::chrono::microseconds timeoutMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(timeoutRemainder);
    
    timeval sendTimeout;
    sendTimeout.tv_sec = (decltype(sendTimeout.tv_sec))timeoutSeconds.count();
    sendTimeout.tv_usec = (decltype(sendTimeout.tv_usec))timeoutMicroseconds.count();
#endif

    int result = SetSockOpt(SOL_SOCKET, SO_SNDTIMEO, (char*)&sendTimeout, sizeof(sendTimeout));
    if (result == SOCKET_ERROR)
    {
        LOG("Failed to set send timeout (%d)", GetSocketError());
        return false;
    }

    return true;
}

int Socket::SetSockOpt(int level, int optname, const char* optval, socklen_t optlen)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    return setsockopt(mSocket, level, optname, optval, optlen);
}

int Socket::GetSockOpt(int level, int optname, char* optval, socklen_t* optlen)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    return getsockopt(mSocket, level, optname, optval, optlen);
}

int Socket::SetNonblocking(bool nonblocking)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    int result = 0;

#ifdef _MSC_VER
    u_long argp = nonblocking ? 1 : 0;
    result = ioctlsocket(mSocket, FIONBIO, &argp);
#else
    if (nonblocking)
    {
        result = fcntl(mSocket, F_SETFL, O_NONBLOCK);
    }
    else
    {
        int opts = fcntl(mSocket, F_GETFL);
        opts = opts & (~O_NONBLOCK);
        result = fcntl(mSocket, F_SETFL, opts);
    }
#endif

    return result;
}

bool Socket::Receive(char* buffer, int length, int& bytesReceived)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (buffer == nullptr || length == 0)
    {
        THROW(InvalidArgumentException);
    }

    bytesReceived = recv(mSocket, buffer, length, 0);
    if (bytesReceived == SOCKET_ERROR)
    {
        int error = GetSocketError();
        if (error == EAGAIN || error == EWOULDBLOCK)
        {
            return false;
        }
        THROW(SocketException, error, PeerAddress.cref(), PeerPort.cref());
    }
    else if (bytesReceived == 0)
    {
        // Peer socket closed
        Detach();
    }

    return bytesReceived > 0;
}

bool Socket::SendWait(const std::chrono::milliseconds& timeout)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    // Set the socket into non-blocking mode
    SetNonblocking(true);

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
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(mSocket, &writefds);

    fd_set exceptfds;
    FD_ZERO(&exceptfds);
    FD_SET(mSocket, &exceptfds);

    // Wait until timeout for writing, tv = NULL for blocking operation.
    int result = select((int)mSocket + 1, nullptr, &writefds, &exceptfds, tv.get());
    if (result == SOCKET_ERROR)
    {
        // select error
        int error = GetSocketError();
        THROW(SocketException, error, PeerAddress.cref(), PeerPort.cref());
    }
    else if (result > 0)
    {
        if (FD_ISSET(mSocket, &writefds))
        {
            return true;
        }

        if (FD_ISSET(mSocket, &exceptfds))
        {
            // Socket exception
            Detach();
        }
    }
    else
    {
        // Timeout
    }

    return false;
}

bool Socket::Send(const char* buffer, int length, int& bytesSent)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (buffer == nullptr || length == 0)
    {
        THROW(InvalidArgumentException);
    }

    // Set the socket into blocking mode
    //SetNonblocking(false);

    int bytesToSend = length;
    const char* ptrBuf = buffer;
    int sent = 0;
    
#ifdef __linux__
    int flag = MSG_NOSIGNAL;
#else
    int flag = 0;
#endif

    while(bytesToSend > 0)
    {
        sent = send(mSocket, ptrBuf, bytesToSend, flag);
        if (sent == SOCKET_ERROR)
        {
            int error = GetSocketError();
            if (error == EAGAIN || error == EWOULDBLOCK)
            {
                bytesSent = (int)(ptrBuf - buffer);
                return false;
            }
            THROW(SocketException, error, PeerAddress.cref(), PeerPort.cref());
        }

        ptrBuf += sent;
        bytesToSend = length - (int)(ptrBuf - buffer);

        if (sent == 0)
        {
            // Pear connection closed
            Detach();
            return false;
        }
    }

    bytesSent = (int)(ptrBuf - buffer);
    return bytesSent > 0;
}

bool Socket::ReceiveFrom(char* buffer, int length, std::string& source, uint16_t& port, int& bytesReceived)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (buffer == nullptr || length == 0)
    {
        THROW(InvalidArgumentException);
    }

    sockaddr_storage senderAddr = { 0 };
    socklen_t ccbSenderAddrSize = sizeof(sockaddr_storage);
    bytesReceived = recvfrom(mSocket, buffer, length, 0, (sockaddr*)&senderAddr, &ccbSenderAddrSize);
    if (bytesReceived == SOCKET_ERROR)
    {
        int error = GetSocketError();
        if (error == EAGAIN || error == EWOULDBLOCK)
        {
            return false;
        }
        THROW(SocketException, error, PeerAddress.cref(), PeerPort.cref());
    }
    else if (bytesReceived == 0)
    {
        // Peer socket closed
        Detach();
        return false;
    }

    if (!GetAddressName(senderAddr, source, port))
    {
        return false;
    }

    GetSocketAddress();
    
    // Set the peer name and port
    PeerAddress.set(source);
    PeerPort.set(port);

    return bytesReceived > 0;
}

bool Socket::SendTo(const char* buffer, int length, const std::string& destination, uint16_t port, int& bytesSent)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (buffer == nullptr || length == 0 || destination.empty())
    {
        THROW(InvalidArgumentException);
    }

    std::shared_ptr<addrinfo> addrInfo = NetworkUtility::GetAddressInfo(destination, port, mAddrInfo->ai_socktype, mAddrInfo->ai_protocol, false);
    if (addrInfo == nullptr)
    {
        return false;
    }

    // Get the max message size of the socket
    int maxMsgSize = length;
#ifdef _MSC_VER
    int optlen = sizeof(maxMsgSize);
    GetSockOpt(SOL_SOCKET, SO_MAX_MSG_SIZE, (char*)&maxMsgSize, &optlen);
#endif

    // Make sure the message size not succeed the socket's max message size
    int bytesToSend = std::min(length, maxMsgSize);
    const char* ptrBuf = buffer;
    int sent = 0;

#ifdef __linux__
    int flag = MSG_NOSIGNAL;
#else
    int flag = 0;
#endif

    while(bytesToSend > 0)
    {
        sent = sendto(mSocket, ptrBuf, bytesToSend, flag, addrInfo->ai_addr, (int)addrInfo->ai_addrlen);
        if (sent == SOCKET_ERROR)
        {
            int error = GetSocketError();
            if (error == EAGAIN || error == EWOULDBLOCK)
            {
                bytesSent = (int)(ptrBuf - buffer);
                return false;
            }
            THROW(SocketException, error, PeerAddress.cref(), PeerPort.cref());
        }

        ptrBuf += sent;
        bytesToSend = length - (int)(ptrBuf - buffer);

        if (sent == 0)
        {
            // Pear connection closed
            Detach();
            return false;
        }
    }

    bytesSent = (int)(ptrBuf - buffer);

    return bytesSent > 0;
}

bool Socket::Attach(const SOCKET& socket)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (socket == INVALID_SOCKET)
    {
        THROW(InvalidArgumentException);
    }

    // Close previous socket
    Detach();
    mSocket = socket;
    return GetSocketAddress();
}

void Socket::Detach()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    mAddrInfo = nullptr;
        
    if(mSocket != INVALID_SOCKET)
    {
#ifdef _MSC_VER
        shutdown(mSocket, SD_BOTH);
        closesocket(mSocket);
        memset(&mSocketInfo, 0, sizeof(mSocketInfo));
#else
        shutdown(mSocket, SHUT_RDWR);
        close(mSocket);
#endif
        mSocket = INVALID_SOCKET;
        mDisconnected();
    }
}

bool Socket::IsValid()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    return mSocket != INVALID_SOCKET;
}

Socket& Socket::Swap(Socket& socket)
{
    std::swap(mSocket, socket.mSocket);
    std::swap(mAddrInfo, socket.mAddrInfo);

#ifdef _MSC_VER
    std::swap(mSocketInfo, socket.mSocketInfo);
    std::swap(mInitialized, socket.mInitialized);
#endif

    socket.GetSocketAddress();
    socket.GetPeerAddress();

    GetSocketAddress();
    GetPeerAddress();

    return *this;
}

bool Socket::GetAddressName(const sockaddr_storage& addr, std::string& name, uint16_t& port)
{
    if (addr.ss_family == AF_INET)
    {
        char address[INET_ADDRSTRLEN + 1] = {};
        name = inet_ntop(AF_INET, &((sockaddr_in*)&addr)->sin_addr, address, INET_ADDRSTRLEN);
        port = Endian::HostOrder(((sockaddr_in*)&addr)->sin_port);
        return true;
    }
    else if (addr.ss_family == AF_INET6)
    {
        char address[INET6_ADDRSTRLEN + 1] = {};
        name = inet_ntop(AF_INET6, &((sockaddr_in6*)&addr)->sin6_addr, address, INET6_ADDRSTRLEN);
        port = Endian::HostOrder(((sockaddr_in6*)&addr)->sin6_port);
        return true;
    }
    
    return false;
}

bool Socket::GetSocketAddress()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    sockaddr_storage socketAddr;
    socklen_t socketAddrSize = sizeof(socketAddr);
    int result = getsockname(mSocket, (sockaddr*)&socketAddr, &socketAddrSize);
    if (result == 0)
    {
        std::string socketName;
        uint16_t socketPort = 0;
        
        if (GetAddressName(socketAddr, socketName, socketPort))
        {
            HostAddress.set(socketName);
            HostPort.set(socketPort);
            return true;
        }
    }
    
    return false;
}

bool Socket::GetPeerAddress()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    sockaddr_storage peerAddr;
    socklen_t peerAddrLength = sizeof(sockaddr_storage);
    int result = getpeername(mSocket, (sockaddr*)&peerAddr, &peerAddrLength);
    if (result == 0)
    {
        std::string peerAddress;
        uint16_t peerPort = 0;
        
        if (GetAddressName(peerAddr, peerAddress, peerPort))
        {
            PeerAddress.set(peerAddress);
            PeerPort.set(peerPort);
            return true;
        }
    }
    
    return false;
}

bool Socket::CreateSocketFromAddress(const std::string& address, uint16_t port, int type, int protocol, bool forBinding)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    std::shared_ptr<addrinfo> addrInfo = NetworkUtility::GetAddressInfo(address, port, type, protocol, forBinding);
    if (addrInfo == nullptr)
    {
        return false;
    }
    
    // Detach if the socket type is not compatible
    if (mAddrInfo == nullptr ||
        mAddrInfo->ai_family != addrInfo->ai_family ||
        mAddrInfo->ai_socktype != addrInfo->ai_socktype ||
        mAddrInfo->ai_protocol != addrInfo->ai_protocol)
    {
        Detach();
    }

    // Create a new socket
    if (mSocket == INVALID_SOCKET)
    {
        mAddrInfo = addrInfo;
        
        // Create a SOCKET for the server to listen for client connections
        mSocket = socket(mAddrInfo->ai_family, mAddrInfo->ai_socktype, mAddrInfo->ai_protocol);
        if (mSocket == INVALID_SOCKET)
        {
            return false;
        }
    }
    
    return true;
}
