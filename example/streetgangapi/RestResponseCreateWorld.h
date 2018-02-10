#pragma once
#ifndef streetgangapi_RestResponseCreateWorld
#define streetgangapi_RestResponseCreateWorld

#include "ResponseCreateWorld.h"


namespace streetgangapi
{
    class RestResponseCreateWorld : public ResponseCreateWorld
    {
    public:
        RestResponseCreateWorld();
        virtual ~RestResponseCreateWorld();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
    };
}


#endif // streetgangapi_RestResponseCreateWorld
