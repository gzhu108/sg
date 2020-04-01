#pragma once
#ifndef sg_microreactor_UdpListener
#define sg_microreactor_UdpListener

#include "Listener.h"
#include "UdpConnection.h"


namespace sg { namespace microreactor
{
    class UdpListener : public Listener
    {
    public:
        UdpListener(std::shared_ptr<UdpSocket> socket, std::shared_ptr<sg::microreactor::Dispatcher> dispatcher);
        virtual ~UdpListener();
        
        virtual bool IsClosed() override;

    protected:
        virtual bool Close() override;
        virtual std::shared_ptr<Connection> Listen() override;
        
    protected:
        std::shared_ptr<UdpSocket> mSocket;
        std::shared_ptr<UdpConnection> mConnection;
    };
}}


#endif // sg_microreactor_UdpListener
