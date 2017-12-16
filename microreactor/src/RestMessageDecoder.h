#pragma once
#ifndef sg_microreactor_RestMessageDecoder
#define sg_microreactor_RestMessageDecoder

#include <map>
#include "Dispatcher.h"
#include "RestFactory.h"


namespace sg { namespace microreactor
{
    class RestMessageDecoder : public Dispatcher
    {
    public:
        RestMessageDecoder();
        virtual ~RestMessageDecoder();

    public:
        virtual void Dispatch(Connection& connection) override;
        virtual void RegisterRestFactory(std::shared_ptr<RestFactory> restFactory);

    protected:
        virtual std::shared_ptr<Reactor> Decode(Connection& connection);
        virtual std::shared_ptr<RestFactory> GetRestFactory(std::shared_ptr<RestRequest> restRequest);
        virtual void PushChunkedRequest(Connection& connection, std::shared_ptr<RestRequest> restRequest);
        virtual std::shared_ptr<RestRequest> PopChunkedRequest(Connection& connection);

    protected:
        std::recursive_mutex mLock;
        std::map<uintptr_t, std::shared_ptr<RestRequest>> mChunkedRequestStore;

        // <URI, RestFactory>
        typedef std::map<std::string, std::shared_ptr<RestFactory>> RestFactoryList;
        // <METHOD, RestFactoryList>
        std::map<std::string, RestFactoryList> mRestFactoryTable;
    };
}}


#endif // sg_microreactor_RestMessageDecoder
