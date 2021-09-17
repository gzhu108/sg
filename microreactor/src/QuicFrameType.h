#pragma once
#ifndef microreactor_QuicFrameType
#define microreactor_QuicFrameType

#include "QuicMessage.h"


namespace microreactor
{
    class QuicFrameType : public QuicMessage
    {
    public:
        QuicFrameType() {}
        virtual ~QuicFrameType() {}

    public:
        virtual char* EncodeMessage(char* first, char* last) const override;
        virtual const char* DecodeMessage(const char* first, const char* last) override;
    };
}


#endif //microreactor_QuicFrameType
