#pragma once
#ifndef sg_microreactor_TcpHost
#define sg_microreactor_TcpHost

#include "Host.h"
#include "TcpSocket.h"

namespace sg { namespace microreactor
{
    class TcpHost : public Host
    {
    public:
        TcpHost(std::shared_ptr<TcpSocket> socket, std::shared_ptr<Profile> profile);
        virtual ~TcpHost();

        virtual bool IsClosed() override;
        
    protected:
        virtual bool Close() override;
        virtual std::shared_ptr<Connection> Listen(const std::chrono::milliseconds& timeout) override;
        
    protected:
        std::shared_ptr<TcpSocket> mSocket;
    };
}}


#endif // sg_microreactor_TcpHost
