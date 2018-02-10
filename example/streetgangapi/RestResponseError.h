#pragma once
#ifndef streetgangapi_RestResponseError
#define streetgangapi_RestResponseError

#include "ResponseError.h"


namespace streetgangapi
{
    class RestResponseError : public ResponseError
    {
    public:
        RestResponseError();
        virtual ~RestResponseError();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
    };
}


#endif // streetgangapi_RestResponseError
