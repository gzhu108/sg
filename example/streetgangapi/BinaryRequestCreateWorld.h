#pragma once
#ifndef streetgangapi_BinaryRequestCreateWorld
#define streetgangapi_BinaryRequestCreateWorld

#include "RequestCreateWorld.h"


namespace streetgangapi
{
    class BinaryRequestCreateWorld : public RequestCreateWorld
    {
    public:
        BinaryRequestCreateWorld();
        virtual ~BinaryRequestCreateWorld();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangapi_BinaryRequestCreateWorld
