#pragma once
#ifndef streetgangapi_BinaryResponseCreateWorld
#define streetgangapi_BinaryResponseCreateWorld

#include "ResponseCreateWorld.h"


namespace streetgangapi
{
    class BinaryResponseCreateWorld : public ResponseCreateWorld
    {
    public:
        BinaryResponseCreateWorld();
        virtual ~BinaryResponseCreateWorld();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangapi_BinaryResponseCreateWorld
