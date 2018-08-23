#pragma once
#ifndef sg_microreactor_RestRequest
#define sg_microreactor_RestRequest

#include "RestMessage.h"


namespace sg { namespace microreactor
{
    class RestRequest : public RestMessage
    {
    public:
        RestRequest() : RestMessage(RestMessage::Request) {}
        virtual ~RestRequest() {}

    public:
        std::string mMethod;
        std::string mUri;

        virtual bool FlushToBuffer(std::string& buffer) override;

        virtual std::shared_ptr<RestMessage> CreateMessage() override
        {
            return std::make_shared<RestRequest>();
        }
    };
}}


#endif // sg_microreactor_RestRequest
