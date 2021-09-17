#pragma once
#ifndef microreactor_QuicMessage
#define microreactor_QuicMessage

#include "Common.h"


namespace microreactor
{
    class QuicMessage
    {
    public:
        QuicMessage() {}
        virtual ~QuicMessage() {}

    public:
        virtual char* EncodeMessage(char* first, char* last) const = 0;
        virtual const char* DecodeMessage(const char* first, const char* last) = 0;
    };
}


#endif //microreactor_QuicMessage
