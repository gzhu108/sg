#pragma once
#ifndef streetgangserver_RequestGetSceneReactor
#define streetgangserver_RequestGetSceneReactor

#include "MessageReactor.h"
#include "StreetGangResponder.h"
#include "worldapi/Point.h"
#include "streetgangapi/Point.h"
#include "streetgangapi/RequestGetScene.h"
#include "StreetGangReactor.h"


namespace streetgangserver
{
    class RequestGetSceneReactor
        : public sg::microreactor::MessageReactor<streetgangapi::RequestGetScene>
        , public StreetGangReactor
    {
    public:
        RequestGetSceneReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::RequestGetScene> message, std::shared_ptr<streetgangapi::StreetGangResponder> responder);
        virtual ~RequestGetSceneReactor();
        
    public:
        virtual bool Process() override;
        virtual bool SendResponse(const streetgangapi::SessionId& sessionId, const std::vector<worldapi::Point<float>>& items);

    protected:
        virtual uint64_t GetItemsInRect(const streetgangapi::Rectangle<float>& rect, const std::vector<worldapi::Point<float>>& sourceItems, std::vector<streetgangapi::Point<float>>& targetItems);
    };
}


#endif // streetgangserver_RequestGetSceneReactor
