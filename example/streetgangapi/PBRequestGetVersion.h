#pragma once
#ifndef streetgangapi_PBRequestGetVersion
#define streetgangapi_PBRequestGetVersion

#include "RequestGetVersion.h"
#include "PBMessage.h"


namespace streetgangapi
{
    class PBRequestGetVersion : public PBMessage<RequestGetVersion>
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
