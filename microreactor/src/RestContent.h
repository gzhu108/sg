#pragma once
#ifndef sg_microreactor_RestContent
#define sg_microreactor_RestContent

#include "Common.h"


namespace sg { namespace microreactor
{

    template <typename ContentSerializer>
    class RestContent
    {
    public:
        virtual bool Encode(ContentSerializer& serializer) const = 0;
        virtual bool Decode(const ContentSerializer& serializer) = 0;
    };

}}


#endif // sg_microreactor_RestContent
