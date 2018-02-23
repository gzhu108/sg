#pragma once
#ifndef streetgangapi_PBRequestGetScene
#define streetgangapi_PBRequestGetScene

#include "RequestGetScene.h"
#include "PBMessage.h"


namespace streetgangapi
{
    class PBRequestGetScene : public PBMessage<RequestGetScene>
    {
    public:
        PBRequestGetScene();
        virtual ~PBRequestGetScene();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangapi_PBRequestGetScene
