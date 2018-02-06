#pragma once
#ifndef myserver_GETv1versionResponse
#define myserver_GETv1versionResponse

#include "RestResponse.h"


namespace myserver
{
    class GETv1versionResponse : public sg::microreactor::RestResponse
    {
    public:
        GETv1versionResponse();
        virtual ~GETv1versionResponse();

        PROPERTY(Version, std::string, "1.0.0.0");
        
    public:
        virtual bool FlushToBuffer(std::string& buffer) override;
    };
}


#endif // myserver_GETv1versionResponse
