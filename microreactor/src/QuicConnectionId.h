#pragma once
#ifndef microreactor_QuicConnectionId
#define microreactor_QuicConnectionId

#include "Common.h"


namespace microreactor
{
    class QuicConnectionId
    {
    public:
        QuicConnectionId();
        virtual ~QuicConnectionId();

    protected:
        uint8_t mLength {0};
        uint8_t mId[256] {0};
    };
}


#endif //microreactor_QuicConnectionId
