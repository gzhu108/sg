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
        virtual bool Serialize(std::ostream& stream) const;
        virtual bool Deserialize(std::istream& stream);
    };
}


#endif // streetgangapi_BinaryRequestCreateWorld
