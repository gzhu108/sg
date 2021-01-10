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
        virtual bool Listen(const std::string& hostAddress, uint16_t port);
        virtual std::shared_ptr<TcpSocket> Accept();
        virtual bool Connect(const std::string& address, uint16_t port);
        
    protected:
        virtual bool SetKeepalive(TcpSocket& socket, int keepalive, int keepaliveProbes, int keepaliveIdleTime, int keepaliveProbeInterval);
    };
}}


#endif //sg_microreactor_TcpSocket
