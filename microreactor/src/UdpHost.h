#pragma once
#ifndef sg_microreactor_UdpHost
#define sg_microreactor_UdpHost

#include "Host.h"
#include "UdpConnection.h"


namespace sg { namespace microreactor
{
    class UdpHost : public Host
    {
    public:
        UdpHost(std::shared_ptr<UdpSocket> socket, std::shared_ptr<Profile> profile);
        virtual ~UdpHost();
        
        virtual bool IsClosed() override;

    protected:
        virtual bool Close() override;
        virtual std::shared_ptr<Connection> Listen(const std::chrono::milliseconds& timeout) override;
        
    protected:
        std::shared_ptr<UdpSocket> mSocket;
        std::shared_ptr<UdpConnection> mConnection;
    };
}}


#endif // sg_microreactor_UdpHost
