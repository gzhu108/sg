#pragma once
#ifndef streetgangapi_BinaryRequestGetScene
#define streetgangapi_BinaryRequestGetScene

#include "RequestGetScene.h"


namespace streetgangapi
{
    class BinaryRequestGetScene : public RequestGetScene
    {
    public:
        BinaryRequestGetScene();
        virtual ~BinaryRequestGetScene();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangapi_BinaryRequestGetScene
