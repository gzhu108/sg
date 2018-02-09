#pragma once
#ifndef streetgangapi_PBResponseGetScene
#define streetgangapi_PBResponseGetScene

#include "ResponseGetScene.h"


namespace streetgangapi
{
    class PBResponseGetScene : public ResponseGetScene
    {
    public:
        PBResponseGetScene();
        virtual ~PBResponseGetScene();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangapi_PBResponseGetScene
