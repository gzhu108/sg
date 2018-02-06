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

        // TODO: Add Response properties here
    };
}


#endif // myserver_GETv1versionResponse
