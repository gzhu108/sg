#pragma once
#ifndef microreactor_Serializable
#define microreactor_Serializable

#include "Serializer.h"


namespace microreactor
{
    class Serializable
    {
    public:
        virtual bool Serialize(Serializer& serializer, std::ostream& stream) const = 0;
        virtual bool Deserialize(std::istream& stream, Serializer& serializer) = 0;
    };
}


#endif // microreactor_Serializable
