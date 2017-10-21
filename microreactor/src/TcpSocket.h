#pragma once
#ifndef sg_microreactor_TcpSocket
#define sg_microreactor_TcpSocket

#include "Socket.h"


namespace sg { namespace microreactor
{
    class TcpSocket : public Socket
    {
    public:
        TcpSocket();
        explicit TcpSocket(SOCKET socket);
        virtual ~TcpSocket();

    public:
        virtual bool Listen(const std::string& hostName, uint16_t port);
        virtual std::shared_ptr<TcpSocket> Accept(const std::chrono::milliseconds& timeout);
        virtual bool Connect(const std::string& address, uint16_t port, const std::chrono::milliseconds& timeout);
        
    protected:
        virtual bool SetKeepalive(TcpSocket& socket, int32_t keepalive, int32_t keepaliveProbes, int32_t keepaliveIdleTime, int32_t keepaliveProbeInterval);
    };
}}


#endif //sg_microreactor_TcpSocket
