#pragma once
#ifndef microreactor_QuicConnection
#define microreactor_QuicConnection

#include <map>
#include "QuicConnectionId.h"
#include "QuicStream.h"

namespace microreactor
{
    class QuicConnection
    {
    public:
        QuicConnection();
        virtual ~QuicConnection();

    protected:
        QuicConnectionId mConnectionId;
        std::map<QuicStreamId, std::shared_ptr<QuicStream>> mStreams;
    };
}


#endif //microreactor_QuicConnection
