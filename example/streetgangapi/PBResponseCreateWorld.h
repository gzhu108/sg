#pragma once
#ifndef streetgangapi_PBResponseCreateWorld
#define streetgangapi_PBResponseCreateWorld

#include "ResponseCreateWorld.h"


namespace streetgangapi
{
    class PBResponseCreateWorld : public ResponseCreateWorld
    {
    public:
        PBResponseCreateWorld();
        virtual ~PBResponseCreateWorld();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangapi_PBResponseCreateWorld
