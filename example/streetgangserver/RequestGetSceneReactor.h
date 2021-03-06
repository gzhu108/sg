#pragma once
#ifndef streetgangserver_RequestGetSceneReactor
#define streetgangserver_RequestGetSceneReactor

#include "MessageReactor.h"
#include "StreetGangResponder.h"
#include "worldapi/Point.h"
#include "worldapi/ResponseGetWorld.h"
#include "worldapi/ResponseError.h"
#include "streetgangapi/Point.h"
#include "streetgangapi/RequestGetScene.h"
#include "StreetGangReactor.h"


namespace streetgangserver
{
    class RequestGetSceneReactor
        : public microreactor::MessageReactor<streetgangapi::RequestGetScene>
        , public StreetGangReactor
    {
    public:
        RequestGetSceneReactor(microreactor::Connection& connection, std::shared_ptr<streetgangapi::RequestGetScene> message, std::shared_ptr<streetgangapi::StreetGangResponder> responder);
        virtual ~RequestGetSceneReactor();
        
    public:
        virtual bool Process() override;
        virtual bool ProcessError(std::shared_ptr<microreactor::Message> errorMessage) override;
        virtual bool ProcessTimeout(std::shared_ptr<microreactor::Message> timedOutMessage) override;

        virtual void CheckComplete() override;

        bool ProcessGetWorldResponse(std::shared_ptr<worldapi::ResponseGetWorld> response);
        bool SendResponse(const streetgangapi::SessionId& sessionId, const std::vector<worldapi::Point<float>>& items);

    protected:
        virtual uint64_t GetItemsInRect(const streetgangapi::Rectangle<float>& rect, const std::vector<worldapi::Point<float>>& sourceItems, std::vector<streetgangapi::Point<float>>& targetItems);

    protected:
        bool mSendComplete = false;
    };
}


#endif // streetgangserver_RequestGetSceneReactor
