#pragma once
#ifndef streetgangapi_RestRequestCreateWorld
#define streetgangapi_RestRequestCreateWorld

#include "RequestCreateWorld.h"
#include "RestRequest.h"


namespace streetgangapi
{
    class RestRequestCreateWorld : public RequestCreateWorld
    {
    public:
        RestRequestCreateWorld();
        virtual ~RestRequestCreateWorld();
        
    public:
        virtual bool Decode(std::shared_ptr<microreactor::RestRequest> request);
    };
}


#endif // streetgangapi_RestRequestCreateWorld
