#pragma once
#ifndef sg_microreactor_RestService
#define sg_microreactor_RestService

#include "Microreactor.h"
#include "HttpHeader.h"
#include "RestApi.h"
#include "RestFileResponse.h"
#include "RestMessageDecoder.h"
#include "RestRequest.h"
#include "RestResponse.h"
#include "UriCodec.h"


namespace sg { namespace microreactor
{
    class RestService : public sg::microreactor::Microservice
    {
    public:
        RestService(const std::string& hostName, uint16_t port);
        explicit RestService(std::shared_ptr<sg::microreactor::Profile> profile);
        RestService(std::shared_ptr<sg::microreactor::Host> host, std::shared_ptr<sg::microreactor::Profile> profile);
        virtual ~RestService();

    public:
        void RegisterRestApi(std::shared_ptr<RestApi> restApi);

    protected:
        std::shared_ptr<RestMessageDecoder> mRestMessageDecoder;
    };
}}


#endif // sg_microreactor_RestService
