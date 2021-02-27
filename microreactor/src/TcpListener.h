#pragma once
#ifndef microreactor_TcpListener
#define microreactor_TcpListener

#include "Listener.h"
#include "TcpSocket.h"

namespace microreactor
{
    class TcpListener : public Listener
    {
    public:
        TcpListener(std::shared_ptr<TcpSocket> socket, std::shared_ptr<microreactor::Dispatcher> dispatcher);
        virtual ~TcpListener();

        virtual bool IsClosed() override;
        
    protected:
        virtual bool Close() override;
        virtual std::shared_ptr<Connection> Listen() override;
        
    protected:
        std::shared_ptr<TcpSocket> mSocket;
    };
}


#endif // microreactor_TcpListener
