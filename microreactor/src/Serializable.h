#pragma once
#ifndef sg_microreactor_Serializable
#define sg_microreactor_Serializable

#include "Serializer.h"


namespace sg { namespace microreactor
{
    class Serializable
    {
    public:
        virtual bool Serialize(Serializer& serializer, std::ostream& stream) const = 0;
        virtual bool Deserialize(std::istream& stream, Serializer& serializer) = 0;
    };
}}


#endif // sg_microreactor_Serializable
