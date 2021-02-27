#pragma once
#ifndef microreactor_RestContent
#define microreactor_RestContent

#include "Common.h"


namespace microreactor
{

    template <typename ContentSerializer>
    class RestContent
    {
    public:
        virtual bool Encode(ContentSerializer& serializer) const = 0;
        virtual bool Decode(const ContentSerializer& serializer) = 0;
    };

}


#endif // microreactor_RestContent
