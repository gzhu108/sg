#pragma once
#ifndef streetgangapi_BinaryResponseError
#define streetgangapi_BinaryResponseError

#include "ResponseError.h"


namespace streetgangapi
{
    class BinaryResponseError : public ResponseError
    {
    public:
        BinaryResponseError();
        virtual ~BinaryResponseError();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangapi_BinaryResponseError
