#pragma once
#ifndef sg_microreactor_TcpEndpoint
#define sg_microreactor_TcpEndpoint

#include "Endpoint.h"
#include "TcpSocket.h"

namespace sg { namespace microreactor
{
    class TcpEndpoint : public Endpoint
    {
    public:
        TcpEndpoint(std::shared_ptr<TcpSocket> socket, std::shared_ptr<Profile> profile);
        virtual ~TcpEndpoint();

        virtual bool IsClosed() override;
        
    protected:
        virtual bool Close() override;
        virtual std::shared_ptr<Connection> Listen(const std::chrono::milliseconds& timeout) override;
        
    protected:
        std::shared_ptr<TcpSocket> mSocket;
    };
}}


#endif // sg_microreactor_TcpEndpoint
