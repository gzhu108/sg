#pragma once
#ifndef streetgangapi_RestResponseGetScene
#define streetgangapi_RestResponseGetScene

#include "ResponseGetScene.h"


namespace streetgangapi
{
    class RestResponseGetScene : public ResponseGetScene
    {
    public:
        RestResponseGetScene();
        virtual ~RestResponseGetScene();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
    };
}


#endif // streetgangapi_RestResponseGetScene
