#pragma once
#ifndef worldapi_RequestError
#define worldapi_RequestError

#include "MessageBase.h"


namespace worldapi
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


#endif // worldapi_RequestError
