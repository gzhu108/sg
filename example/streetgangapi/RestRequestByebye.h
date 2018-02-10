#pragma once
#ifndef streetgangapi_RestRequestByebye
#define streetgangapi_RestRequestByebye

#include "RequestByebye.h"
#include "RestRequest.h"


namespace streetgangapi
{
    class RestRequestByebye : public RequestByebye
    {
    public:
        RestRequestByebye();
        virtual ~RestRequestByebye();
        
    public:
        virtual bool Decode(std::shared_ptr<sg::microreactor::RestRequest> request);
    };
}


#endif // streetgangapi_RestRequestByebye
