#pragma once
#ifndef streetgangapi_BinaryRequestGetVersion
#define streetgangapi_BinaryRequestGetVersion

#include "RequestGetVersion.h"


namespace streetgangapi
{
    class BinaryRequestGetVersion : public RequestGetVersion
    {
    public:
        BinaryRequestGetVersion();
        virtual ~BinaryRequestGetVersion();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangapi_BinaryRequestGetVersion
