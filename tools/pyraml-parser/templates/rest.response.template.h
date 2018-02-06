#pragma once
#ifndef $namespace_$method$funcResponse
#define $namespace_$method$funcResponse

#include "RestResponse.h"


namespace $namespace
{
    class $method$funcResponse : public sg::microreactor::RestResponse
    {
    public:
        $method$funcResponse();
        virtual ~$method$funcResponse();

        @property_begin
        PROPERTY($propname, $proptype, $propvalue);
        @property_end
        
    public:
        virtual bool FlushToBuffer(std::string& buffer) override;
    };
}


#endif // $namespace_$method$funcResponse
