#pragma once
#ifndef microreactor_RestRequest
#define microreactor_RestRequest

#include "RestMessage.h"


namespace microreactor
{
    class RestRequest : public RestMessage
    {
    public:
        RestRequest() : RestMessage(RestMessage::Request) {}
        virtual ~RestRequest() {}

    public:
        std::string mMethod;
        std::string mUri;

        virtual bool Write(std::string& buffer) const override;

        virtual std::shared_ptr<RestMessage> CreateMessage() override
        {
            return std::make_shared<RestRequest>();
        }
    };
}


#endif // microreactor_RestRequest
