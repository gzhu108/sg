#pragma once
#ifndef microreactor_RestDispatcher
#define microreactor_RestDispatcher

#include "Dispatcher.h"
#include "ReactorFactoryContainer.h"
#include "RestRequest.h"
#include "RestResponse.h"


namespace microreactor
{
    class RestDispatcher
        : public Dispatcher
        , public ReactorFactoryContainer<std::string, std::shared_ptr<RestMessage>>
    {
    public:
        RestDispatcher();
        virtual ~RestDispatcher();

    public:
        virtual void Dispatch(Connection& connection) override;
        virtual void RegisterResource(const std::string& method, const std::string& uri, Factory factory);

    protected:
        virtual std::shared_ptr<Reactor> Decode(Connection& connection);
        virtual std::shared_ptr<Reactor> CreateReactor(std::shared_ptr<RestMessage> restMessage, Connection& connection);
        virtual Factory GetRestReactorFactory(std::shared_ptr<RestRequest> restRequest);
        virtual Factory GetRestReactorFactory(std::shared_ptr<RestResponse> restResponse);

        virtual void PushChunkedMessage(Connection& connection, std::shared_ptr<RestMessage> restMessage);
        virtual std::shared_ptr<RestMessage> PopChunkedMessage(Connection& connection);

        virtual void SendBadMessageResponse(Connection& connection);
        virtual void SendNotFoundResponse(Connection& connection);

    protected:
        std::recursive_mutex mLock;
        std::map<uintptr_t, std::shared_ptr<RestMessage>> mChunkedMessageStore;
    };
}


#endif // microreactor_RestDispatcher
