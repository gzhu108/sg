#pragma once
#ifndef worldapi_WorldMessage
#define worldapi_WorldMessage

#include "Message.h"
#include "WorldId.h"


namespace worldapi
{
    class WorldMessage : public microreactor::Message
    {
    public:
        WorldMessage();
        virtual ~WorldMessage();

        PROPERTY(Id, int32_t, (int32_t)ID::Unknown);
        PROPERTY(Result, int32_t, (int32_t)ResultCode::Unknown);
    };
}


#endif // worldapi_WorldMessage
