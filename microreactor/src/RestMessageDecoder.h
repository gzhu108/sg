#pragma once
#ifndef sg_microreactor_RestMessageDecoder
#define sg_microreactor_RestMessageDecoder

#include <map>
#include "Dispatcher.h"
#include "RestApi.h"


namespace sg { namespace microreactor
{
    class RestMessageDecoder : public Dispatcher
    {
    public:
        RestMessageDecoder();
        virtual ~RestMessageDecoder();

    public:
        virtual void Dispatch(Connection& connection) override;
        virtual void RegisterRestApi(std::shared_ptr<RestApi> restApi);

    protected:
        virtual std::shared_ptr<Reactor> Decode(Connection& connection);
        virtual std::shared_ptr<RestApi> GetRestApi(std::shared_ptr<RestRequest> restRequest);
        virtual void PushChunkedRequest(Connection& connection, std::shared_ptr<RestRequest> restRequest);
        virtual std::shared_ptr<RestRequest> PopChunkedRequest(Connection& connection);

    protected:
        std::recursive_mutex mLock;
        std::map<uintptr_t, std::shared_ptr<RestRequest>> mChunkedRequestStore;

        // <URI, RestApi>
        typedef std::map<std::string, std::shared_ptr<RestApi>> RestApiList;
        // <METHOD, RestApiList>
        std::map<std::string, RestApiList> mRestApiTable;
    };
}}


#endif // sg_microreactor_RestMessageDecoder
