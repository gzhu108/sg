#pragma once
#ifndef $namespace_$method$funcMessage
#define $namespace_$method$funcMessage

#include "RestMessage.h"


namespace $namespace
{
    class $method$funcMessage : public sg::microreactor::RestMessage
    {
    public:
        $method$funcMessage(std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~$method$funcMessage();

        @property_begin
        PROPERTY($propname, $proptype, $propvalue);
        @property_end
    };
}


#endif // $namespace_$method$funcMessage
