#pragma once
#ifndef microreactor_UdpListener
#define microreactor_UdpListener

#include "Listener.h"
#include "UdpConnection.h"


namespace microreactor
{
    class UdpListener : public Listener
    {
    public:
        UdpListener(std::shared_ptr<UdpSocket> socket, std::shared_ptr<microreactor::Dispatcher> dispatcher);
        virtual ~UdpListener();
        
        virtual bool IsClosed() override;

    protected:
        virtual bool Close() override;
        virtual std::shared_ptr<Connection> Listen() override;
        
    protected:
        std::shared_ptr<UdpSocket> mSocket;
        std::shared_ptr<UdpConnection> mConnection;
    };
}


#endif // microreactor_UdpListener
