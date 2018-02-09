#pragma once
#ifndef streetgangapi_PBRequestGetVersion
#define streetgangapi_PBRequestGetVersion

#include "RequestGetVersion.h"


namespace streetgangapi
{
    class PBRequestGetVersion : public RequestGetVersion
    {
    public:
        PBRequestGetVersion();
        virtual ~PBRequestGetVersion();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangapi_PBRequestGetVersion
