#pragma once
#ifndef microreactor_Socket
#define microreactor_Socket

#ifdef _MSC_VER

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <Mstcpip.h>
#include <Mswsock.h>
#pragma comment(lib, "Ws2_32.lib")

#ifdef EWOULDBLOCK
#undef EWOULDBLOCK
#endif
#define EWOULDBLOCK WSAEWOULDBLOCK

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
typedef int SOCKET;

#endif

#include <chrono>
#include "Common.h"

#define DEFAULT_SOCKET_BUFLEN 2048


namespace microreactor
{
    class Socket
    {
    public:
        Socket();
        virtual ~Socket();

    public:
        Signal<void>& Connected = mConnected;
        Signal<void>& Disconnected = mDisconnected;
        
        PROPERTY(HostAddress, std::string);
        PROPERTY(HostPort, uint16_t, 0);
        
        PROPERTY(PeerAddress, std::string);
        PROPERTY(PeerPort, uint16_t, 0);

    public:
        virtual void SetAddrInfo(std::shared_ptr<addrinfo> addrInfo);

        virtual bool SetReceiveBufferSize(uint32_t sizeInBytes);
        virtual bool SetSendBufferSize(uint32_t sizeInBytes);

        virtual bool SetReceiveTimeout(const std::chrono::milliseconds& timeout);
        virtual bool SetSendTimeout(const std::chrono::milliseconds& timeout);

        virtual int SetSockOpt(int level, int optname, const char* optval, socklen_t optlen);
        virtual int GetSockOpt(int level, int optname, char* optval, socklen_t* optlen);

        virtual int SetNonblocking(bool nonblocking);
        virtual bool SendWait(const std::chrono::milliseconds& timeout);

        virtual bool Receive(char* buffer, int length, int& bytesReceived);
        virtual bool Send(const char* buffer, int length, int& bytesSent);

        virtual bool ReceiveFrom(char* buffer, int length, std::string& source, uint16_t& port, int& bytesReceived);
        virtual bool SendTo(const char* buffer, int length, const std::string& destination, uint16_t port, int& bytesSent);

        virtual void Detach();
        virtual bool IsValid();
        virtual Socket& Swap(Socket& socket);

        static bool GetAddressName(const sockaddr_storage& addr, std::string& name, uint16_t& port);

    protected:
        virtual bool Attach(const SOCKET& socket);
        virtual bool GetSocketAddress();
        virtual bool GetPeerAddress();
        virtual bool CreateSocketFromAddress(const std::string& address, uint16_t port, int type, int protocol, bool forBinding);

    protected:
        std::recursive_mutex mLock;
        SOCKET mSocket;
        std::shared_ptr<addrinfo> mAddrInfo;
        Emittable<void> mConnected;
        Emittable<void> mDisconnected;
        
#ifdef _MSC_VER
        WSADATA mSocketInfo;
        bool mInitialized { false };
#endif
    };
}


#endif //microreactor_Socket
