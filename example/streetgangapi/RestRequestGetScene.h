#pragma once
#ifndef streetgangapi_RestRequestGetScene
#define streetgangapi_RestRequestGetScene

#include "RequestGetScene.h"
#include "RestRequest.h"


namespace streetgangapi
{
    class RestRequestGetScene : public RequestGetScene
    {
    public:
        RestRequestGetScene();
        virtual ~RestRequestGetScene();
        
    public:
        virtual bool Decode(std::shared_ptr<sg::microreactor::RestRequest> request);
    };
}


#endif // streetgangapi_RestRequestGetScene
