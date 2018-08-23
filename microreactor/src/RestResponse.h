#pragma once
#ifndef sg_microreactor_RestResponse
#define sg_microreactor_RestResponse

#include "RestMessage.h"


namespace sg { namespace microreactor
{
    class RestResponse : public RestMessage
    {
    public:
        RestResponse() : RestMessage(RestMessage::Response) {}
        virtual ~RestResponse() {}

    public:
        uint32_t mStatusCode = 200;
        std::string mStatusText = "OK";

        virtual bool FlushToBuffer(std::string& buffer) override;

        virtual std::shared_ptr<RestMessage> CreateMessage() override
        {
            return std::make_shared<RestResponse>();
        }

        static bool SendWith(Connection& connection, uint32_t statusCode, const std::string& statusText, const std::vector<HttpHeader>& headers = {}, std::shared_ptr<std::string> body = {}, bool connectionClosed = false);       
        
        inline static bool SendErrorWith(Connection& connection, uint32_t statusCode, const std::string& statusText, const std::vector<HttpHeader>& headers = {}, std::shared_ptr<std::string> body = {})
        {
            return SendWith(connection, statusCode, statusText, headers, body, true);
        }
    };
}}


#endif // sg_microreactor_RestResponse
