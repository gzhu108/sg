#pragma once
#ifndef streetgangapi_RestRequestGetVersion
#define streetgangapi_RestRequestGetVersion

#include "RequestGetVersion.h"
#include "RestRequest.h"


namespace streetgangapi
{
    class RestRequestGetVersion : public RequestGetVersion
    {
    public:
        RestRequestGetVersion();
        virtual ~RestRequestGetVersion();
        
    public:
        virtual bool Decode(std::shared_ptr<sg::microreactor::RestRequest> request);
    };
}


#endif // streetgangapi_RestRequestGetVersion
