#pragma once
#ifndef sg_microreactor_Socket
#define sg_microreactor_Socket

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
typedef int32_t SOCKET;

#endif

#include <chrono>
#include "Common.h"

#define DEFAULT_SOCKET_BUFLEN 2048


namespace sg { namespace microreactor
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

        virtual int32_t SetSockOpt(int32_t level, int32_t optname, const char* optval, socklen_t optlen);
        virtual int32_t GetSockOpt(int32_t level, int32_t optname, char* optval, socklen_t* optlen);

        virtual int32_t SetNonblocking(bool nonblocking);
        virtual bool SendWait(const std::chrono::milliseconds& timeout);

        virtual bool Receive(char* buffer, int32_t length, int32_t& bytesReceived);
        virtual bool Send(const char* buffer, int32_t length, int32_t& bytesSent);

        virtual bool ReceiveFrom(char* buffer, int32_t length, std::string& source, uint16_t& port, int32_t& bytesReceived);
        virtual bool SendTo(const char* buffer, int32_t length, const std::string& destination, uint16_t port, int32_t& bytesSent);

        virtual void Detach();
        virtual bool IsValid();
        virtual Socket& Swap(Socket& socket);

        static bool GetAddressName(const sockaddr_storage& addr, std::string& name, uint16_t& port);

    protected:
        virtual bool Attach(const SOCKET& socket);
        virtual bool GetSocketAddress();
        virtual bool GetPeerAddress();
        virtual bool CreateSocketFromAddress(const std::string& address, uint16_t port, int32_t type, int32_t protocol, bool forBinding);

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
}}


#endif //sg_microreactor_Socket
