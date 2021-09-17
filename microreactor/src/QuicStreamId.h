#pragma once
#ifndef microreactor_QuicStreamId
#define microreactor_QuicStreamId

#include "QuicMessage.h"


namespace microreactor
{
    class QuicStreamId : public QuicMessage
    {
    public:
        enum StreamType : uint8_t
        {
            ClientInitiatedBidirectional=0x00,
            ServerInitiatedBidirectional=0x01,
            ClientInitiatedUnidirectional=0x02,
            ServerInitiatedUnidirectional=0x03,
        };

        QuicStreamId() {}
        QuicStreamId(StreamType streamType, uint64_t value) { mId = value << 2 & (uint64_t)streamType; }
        virtual ~QuicStreamId() {}

    public:
        virtual char* EncodeMessage(char* first, char* last) const override;
        virtual const char* DecodeMessage(const char* first, const char* last) override;

        StreamType Type() const { return (StreamType)(mId << 62 >> 62); }
        uint64_t Value() const { return mId >> 2; }
        const uint64_t& Id() const { return mId; }

    protected:
        uint64_t mId {0};
    };
}


#endif //microreactor_QuicStreamId
