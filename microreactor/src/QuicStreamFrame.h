#pragma once
#ifndef microreactor_QuicStreamFrame
#define microreactor_QuicStreamFrame

#include "QuicFrame.h"
#include "QuicStreamId.h"


namespace microreactor
{
    class QuicStreamFrame : public QuicFrame
    {
    public:
        QuicStreamFrame() {}
        virtual ~QuicStreamFrame() {}

    public:
        virtual char* EncodeMessage(char* first, char* last) const override;
        virtual const char* DecodeMessage(const char* first, const char* last) override;

        const QuicStreamId& StreamId() const { return mStreamId; }
        const uint64_t& Offset() const { return mOffset; }
        const uint64_t& Length() const { return mLength; }
        const std::vector<char>& Buffer() { return mBuffer; }

        QuicStreamFrame& SetStreamId(const QuicStreamId& streamId) { mStreamId = streamId; }
        QuicStreamFrame& SetBuffer(uint64_t offset, uint64_t length, char* buffer);
        bool operator <(const QuicStreamFrame& other) const;

    protected:
        QuicStreamId mStreamId;
        uint64_t mOffset {0};
        uint64_t mLength {0};
        std::vector<char> mBuffer;
    };
}


#endif //microreactor_QuicStreamFrame
