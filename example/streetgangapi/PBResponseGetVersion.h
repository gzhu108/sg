#pragma once
#ifndef streetgangapi_PBResponseGetVersion
#define streetgangapi_PBResponseGetVersion

#include "ResponseGetVersion.h"


namespace streetgangapi
{
    class PBResponseGetVersion : public ResponseGetVersion
    {
    public:
        PBResponseGetVersion();
        virtual ~PBResponseGetVersion();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangapi_PBResponseGetVersion
