#pragma once
#ifndef sg_microreactor_RestService
#define sg_microreactor_RestService

#include "Microreactor.h"
#include "HttpHeader.h"
#include "RestFileResponse.h"
#include "RestMessageDecoder.h"
#include "RestRequest.h"
#include "RestResponse.h"
#include "UriCodec.h"


namespace sg { namespace microreactor
{
    class RestService : public Microservice
    {
    public:
        RestService(const std::string& hostName, uint16_t port);
        explicit RestService(std::shared_ptr<Profile> profile);
        RestService(std::shared_ptr<Endpoint> endpoint, std::shared_ptr<Profile> profile);
        virtual ~RestService();

    public:
        virtual bool Initialize() override;

    protected:
        std::shared_ptr<RestMessageDecoder> mRestMessageDecoder;
    };
}}


#endif // sg_microreactor_RestService
