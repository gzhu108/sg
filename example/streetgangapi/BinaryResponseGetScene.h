#pragma once
#ifndef streetgangapi_BinaryResponseGetScene
#define streetgangapi_BinaryResponseGetScene

#include "ResponseGetScene.h"


namespace streetgangapi
{
    class BinaryResponseGetScene : public ResponseGetScene
    {
    public:
        BinaryResponseGetScene();
        virtual ~BinaryResponseGetScene();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangapi_BinaryResponseGetScene
