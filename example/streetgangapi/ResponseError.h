#pragma once
#ifndef streetgangapi_RequestError
#define streetgangapi_RequestError

#include "MessageBase.h"


namespace streetgangapi
{
    class ResponseError : public MessageBase
    {
    public:
        ResponseError();
        virtual ~ResponseError();
        
        PROPERTY(RequestId, int32_t, (int32_t)ID::Unknown);
        PROPERTY(ErrorMessage, std::string);
    };
}


#endif // streetgangapi_RequestError
