#pragma once
#ifndef sg_microreactor_RestDispatcher
#define sg_microreactor_RestDispatcher

#include "MessageDispatcher.h"
#include "Reactor.h"
#include "RestRequest.h"
#include "RestResponse.h"


namespace sg { namespace microreactor
{
    class RestDispatcher : public MessageDispatcher<std::string, std::shared_ptr<RestMessage>>
    {
    public:
        RestDispatcher();
        virtual ~RestDispatcher();

    public:
        virtual void Dispatch(Connection& connection) override;
        virtual void RegisterRestReactorFactory(const std::string& method, const std::string& uri, Factory factory);

    protected:
        virtual std::shared_ptr<Reactor> Decode(std::istream& stream, Connection& connection) override { return nullptr; }
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
}}


#endif // sg_microreactor_RestDispatcher
