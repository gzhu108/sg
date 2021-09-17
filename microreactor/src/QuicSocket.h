#pragma once
#ifndef microreactor_QuicSocket
#define microreactor_QuicSocket

#include <map>
#include "UdpSocket.h"
#include "QuicConnectionId.h"
#include "QuicConnection.h"


namespace microreactor
{
    class QuicSocket : public UdpSocket
    {
    public:
        QuicSocket();
        virtual ~QuicSocket();

    protected:
        std::map<QuicConnectionId, std::shared_ptr<QuicConnection>> mConnections;
    };
}


#endif //microreactor_QuicSocket
