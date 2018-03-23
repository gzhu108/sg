#pragma once
#ifndef streetgangworldapi_MessageBase
#define streetgangworldapi_MessageBase

#include "Message.h"
#include "WorldId.h"


namespace streetgangworldapi
{
    class MessageBase : public sg::microreactor::Message
    {
    public:
        MessageBase();
        virtual ~MessageBase();

        PROPERTY(Id, int32_t, (int32_t)ID::Unknown);
        PROPERTY(Result, int32_t, (int32_t)ResultCode::Unknown);
    };
}


#endif // streetgangworldapi_MessageBase
