#pragma once
#ifndef streetgangworldapi_RequestError
#define streetgangworldapi_RequestError

#include "MessageBase.h"


namespace streetgangworldapi
{
    class ResponseError : public MessageBase
    {
    public:
        ResponseError();
        virtual ~ResponseError();
        
        PROPERTY(RequestId, int32_t, (int32_t)ID::Unknown);
        PROPERTY(ErrorMessage, std::string);

    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangworldapi_RequestError
