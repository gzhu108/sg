#pragma once
#ifndef myserver_GETv1versionMessage
#define myserver_GETv1versionMessage

#include "RestMessage.h"


namespace myserver
{
    class GETv1versionMessage : public sg::microreactor::RestMessage
    {
    public:
        GETv1versionMessage(std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~GETv1versionMessage();

    };
}


#endif // myserver_GETv1versionMessage
