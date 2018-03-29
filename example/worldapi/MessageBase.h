#pragma once
#ifndef worldapi_MessageBase
#define worldapi_MessageBase

#include "Message.h"
#include "WorldId.h"


namespace worldapi
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


#endif // worldapi_MessageBase
