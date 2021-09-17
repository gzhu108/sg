#pragma once
#ifndef microreactor_QuicFrame
#define microreactor_QuicFrame

#include "QuicFrameType.h"


namespace microreactor
{
    class QuicFrame : public QuicMessage
    {
    public:
        QuicFrame() {}
        virtual ~QuicFrame() {}

    public:
        virtual char* EncodeMessage(char* first, char* last) const override;
        virtual const char* DecodeMessage(const char* first, const char* last) override;

    protected:
        QuicFrameType mFrameType;
    };
}


#endif //microreactor_QuicFrame
