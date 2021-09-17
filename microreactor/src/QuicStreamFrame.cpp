#include "QuicStreamFrame.h"
#include "QuicVariableLengthInteger.h"

using namespace microreactor;


char* QuicStreamFrame::EncodeMessage(char* first, char* last) const
{
    first = QuicFrame::EncodeMessage(first, last);
    if (first < last)
    {
        QuicVariableLengthInteger offset(mOffset);
        first = offset.EncodeMessage(first, last);
        if (first < last)
        {
            QuicVariableLengthInteger length(mLength);
            first = length.EncodeMessage(first, last);
            if (first < last)
            {
                auto copySize = std::min(std::distance(first, last), std::distance(mBuffer.begin(), mBuffer.end()));
                std::copy_n(mBuffer.begin(), copySize, first);
                first += copySize;
            }
        }
    }

    return first;
}

const char* QuicStreamFrame::DecodeMessage(const char* first, const char* last)
{
    QuicVariableLengthInteger offset;
    first = offset.DecodeMessage(first, last);
    if (first != last)
    {
        mOffset = offset.Value();

        QuicVariableLengthInteger length;
        first = length.DecodeMessage(first, last);
        if (first != last)
        {
            mLength = length.Value();

            auto copySize = std::distance(first, last);
            mBuffer.resize(copySize);          
            std::copy_n(first, copySize, mBuffer.begin());
            first += copySize;
        }
    }

    return first;
}

QuicStreamFrame& QuicStreamFrame::SetBuffer(uint64_t offset, uint64_t length, char* buffer)
{
    mOffset = offset;
    mLength = length;
    mBuffer.assign(buffer, buffer + length);
    return *this;
}

bool QuicStreamFrame::operator <(const QuicStreamFrame& other) const
{
    return mOffset < other.mOffset;
}
