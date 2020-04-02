#pragma once
#ifndef myserver_GETv1versionMessage
#define myserver_GETv1versionMessage

#include "RestRequest.h"


namespace myserver
{
    class GETv1versionMessage : public sg::microreactor::Message
    {
    public:
        GETv1versionMessage(std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~GETv1versionMessage();

        PROPERTY(Version, std::string, "1.0.0.0");

    public:
        virtual bool Write(std::string& buffer) const override;
    };
}


#endif // myserver_GETv1versionMessage
