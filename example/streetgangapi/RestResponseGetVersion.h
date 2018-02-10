#pragma once
#ifndef streetgangapi_RestResponseGetVersion
#define streetgangapi_RestResponseGetVersion

#include "ResponseGetVersion.h"


namespace streetgangapi
{
    class RestResponseGetVersion : public ResponseGetVersion
    {
    public:
        RestResponseGetVersion();
        virtual ~RestResponseGetVersion();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
    };
}


#endif // streetgangapi_RestResponseGetVersion
