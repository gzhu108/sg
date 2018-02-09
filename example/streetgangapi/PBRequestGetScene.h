#pragma once
#ifndef streetgangapi_PBRequestGetScene
#define streetgangapi_PBRequestGetScene

#include "RequestGetScene.h"


namespace streetgangapi
{
    class PBRequestGetScene : public RequestGetScene
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
