#pragma once
#ifndef streetgangclient_ResponseGetSceneReactor
#define streetgangclient_ResponseGetSceneReactor

#include "MessageReactor.h"
#include "StreetGangRequester.h"
#include "ResponseGetScene.h"


namespace streetgangclient
{
    class ResponseGetSceneReactor : public microreactor::Reactor
    {
    public:
        ResponseGetSceneReactor(microreactor::Connection& connection, std::shared_ptr<streetgangapi::StreetGangRequester> requester);
        virtual ~ResponseGetSceneReactor();
        
    public:
        virtual bool ProcessError(std::shared_ptr<microreactor::Message> errorMessage) override;
        virtual bool ProcessTimeout(std::shared_ptr<microreactor::Message> timedOutMessage) override;

        bool Process(std::shared_ptr<streetgangapi::ResponseGetScene> response);
        void SendNextRequest();

    protected:
        std::shared_ptr<streetgangapi::StreetGangRequester> mRequester;
    };
}


#endif // streetgangclient_ResponseGetSceneReactor
