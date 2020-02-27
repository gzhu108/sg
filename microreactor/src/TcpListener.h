#pragma once
#ifndef sg_microreactor_TcpListener
#define sg_microreactor_TcpListener

#include "Listener.h"
#include "TcpSocket.h"

namespace sg { namespace microreactor
{
    class TcpListener : public Listener
    {
    public:
        TcpListener(std::shared_ptr<TcpSocket> socket, std::shared_ptr<sg::microreactor::Dispatcher> dispatcher);
        virtual ~TcpListener();

        virtual bool IsClosed() override;
        
    protected:
        virtual bool Close() override;
        virtual std::shared_ptr<Connection> Listen(const std::chrono::milliseconds& timeout) override;
        
    protected:
        std::shared_ptr<TcpSocket> mSocket;
    };
}}


#endif // sg_microreactor_TcpListener
