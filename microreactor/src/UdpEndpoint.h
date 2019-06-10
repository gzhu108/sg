#pragma once
#ifndef sg_microreactor_UdpEndpoint
#define sg_microreactor_UdpEndpoint

#include "Endpoint.h"
#include "UdpConnection.h"


namespace sg { namespace microreactor
{
    class UdpEndpoint : public Endpoint
    {
    public:
        UdpEndpoint(std::shared_ptr<UdpSocket> socket, std::shared_ptr<sg::microreactor::Dispatcher> dispatcher);
        virtual ~UdpEndpoint();
        
        virtual bool IsClosed() override;

    protected:
        virtual bool Close() override;
        virtual std::shared_ptr<Connection> Listen(const std::chrono::milliseconds& timeout) override;
        
    protected:
        std::shared_ptr<UdpSocket> mSocket;
        std::shared_ptr<UdpConnection> mConnection;
    };
}}


#endif // sg_microreactor_UdpEndpoint
