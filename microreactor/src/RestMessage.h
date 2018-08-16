#pragma once
#ifndef sg_microreactor_RestMessage
#define sg_microreactor_RestMessage

#include <string>
#include <vector>
#include "Message.h"
#include "RestRequest.h"


namespace sg { namespace microreactor
{
    class RestMessage : public Message
    {
    public:
        RestMessage(std::shared_ptr<RestRequest> request) : mRestRequest(request) {}
        virtual ~RestMessage() {}

    public:
        virtual std::shared_ptr<RestRequest> Request()
        {
            return mRestRequest;
        }

    protected:
        std::shared_ptr<RestRequest> mRestRequest;
    };
}}


#endif // sg_microreactor_RestMessage
