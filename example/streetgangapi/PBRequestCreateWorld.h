#pragma once
#ifndef streetgangapi_PBRequestCreateWorld
#define streetgangapi_PBRequestCreateWorld

#include "RequestCreateWorld.h"
#include "PBMessage.h"


namespace streetgangapi
{
    class PBRequestCreateWorld : public PBMessage<RequestCreateWorld>
    {
    public:
        PBRequestCreateWorld();
        virtual ~PBRequestCreateWorld();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangapi_PBRequestCreateWorld
