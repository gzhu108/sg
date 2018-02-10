#pragma once
#ifndef sg_microreactor_RestDispatcher
#define sg_microreactor_RestDispatcher

#include "Dispatcher.h"
#include "Reactor.h"
#include "RestRequest.h"


namespace sg { namespace microreactor
{
    typedef std::function<std::shared_ptr<Reactor> (std::shared_ptr<RestRequest>, Connection&)> RestReactorFactory;

    class RestDispatcher : public Dispatcher
    {
    public:
        RestDispatcher();
        virtual ~RestDispatcher();

    public:
        virtual void Dispatch(Connection& connection) override;
        virtual void RegisterRestReactorFactory(const std::string& method, const std::string& uri, RestReactorFactory factory);

    protected:
        virtual std::shared_ptr<Reactor> Decode(Connection& connection);
        virtual RestReactorFactory GetRestReactorFactory(std::shared_ptr<RestRequest> restRequest);

        virtual void PushChunkedRequest(Connection& connection, std::shared_ptr<RestRequest> restRequest);
        virtual std::shared_ptr<RestRequest> PopChunkedRequest(Connection& connection);

    protected:
        std::recursive_mutex mLock;
        std::map<uintptr_t, std::shared_ptr<RestRequest>> mChunkedRequestStore;

        // <URI, RestReactorFactory>
        typedef std::map<std::string, RestReactorFactory> FactoryMap;
        // <METHOD, RestReactorFactoryMap>
        typedef std::map<std::string, FactoryMap> MethodMap;

        MethodMap mRestReactorFactoryTable;
    };
}}


#endif // sg_microreactor_RestDispatcher
