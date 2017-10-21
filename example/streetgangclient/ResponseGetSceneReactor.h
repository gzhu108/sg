#pragma once
#ifndef streetgangclient_ResponseGetSceneReactor
#define streetgangclient_ResponseGetSceneReactor

#include "MessageReactor.h"
#include "StreetGangRequestEncoder.h"
#include "ResponseGetScene.h"


namespace streetgangclient
{
    class ResponseGetSceneReactor
        : public sg::microreactor::MessageReactor<streetgangapi::ResponseGetScene, streetgangapi::StreetGangRequestEncoder>
        , public std::enable_shared_from_this<ResponseGetSceneReactor>
    {
    public:
        ResponseGetSceneReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::ResponseGetScene> message);
        virtual ~ResponseGetSceneReactor();
        
    public:
        virtual bool Process() override;
        void SendNextRequest();
    };
}


#endif // streetgangclient_ResponseGetSceneReactor
