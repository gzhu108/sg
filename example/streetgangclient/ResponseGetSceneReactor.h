#pragma once
#ifndef streetgangclient_ResponseGetSceneReactor
#define streetgangclient_ResponseGetSceneReactor

#include "MessageReactor.h"
#include "StreetGangRequestEncoder.h"
#include "StreetGangRequester.h"
#include "ResponseGetScene.h"


namespace streetgangclient
{
    class ResponseGetSceneReactor : public sg::microreactor::MessageReactor<streetgangapi::ResponseGetScene, streetgangapi::StreetGangRequestEncoder>
    {
    public:
        ResponseGetSceneReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::ResponseGetScene> message, std::shared_ptr<streetgangapi::StreetGangRequester> requester);
        virtual ~ResponseGetSceneReactor();
        
    public:
        virtual bool Process() override;
        void SendNextRequest();

    protected:
        std::shared_ptr<streetgangapi::StreetGangRequester> mRequester;
    };
}


#endif // streetgangclient_ResponseGetSceneReactor
