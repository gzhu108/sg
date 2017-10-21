#pragma once
#ifndef streetgangapi_MessageBase
#define streetgangapi_MessageBase

#include "Message.h"
#include "SessionId.h"
#include "StreetGangIds.h"


namespace streetgangapi
{
    class MessageBase : public sg::microreactor::Message
    {
    public:
        MessageBase();
        virtual ~MessageBase();

        PROPERTY(Id, int32_t, (int32_t)ID::Unknown);
        PROPERTY(Result, int32_t, (int32_t)ResultCode::Unknown);
        PROPERTY(MessageSessionId, SessionId, DefaultSessionId);
    };
}


#endif // streetgangapi_MessageBase
