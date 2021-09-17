#pragma once
#ifndef microreactor_QuicStream
#define microreactor_QuicStream

#include <set>
#include "QuicStreamId.h"
#include "QuicStreamFrame.h"


namespace microreactor
{
    class QuicStream
    {
    public:
        QuicStream();
        virtual ~QuicStream();

    public:
        const QuicStreamId& StreamId() const { return mStreamId; }

    protected:
        QuicStreamId mStreamId;
        std::set<QuicStreamFrame> mInBuffers;
        std::set<QuicStreamFrame> mOutBuffers;
    };
}


#endif //microreactor_QuicStream
