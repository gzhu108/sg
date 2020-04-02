#pragma once
#ifndef myserver_POSTv1updateMessage
#define myserver_POSTv1updateMessage

#include "RestRequest.h"


namespace myserver
{
    class POSTv1updateMessage : public sg::microreactor::Message
    {
    public:
        POSTv1updateMessage(std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~POSTv1updateMessage();

        PROPERTY(Name, std::string, "");
        PROPERTY(Age, std::string, "18");

    public:
        virtual bool Write(std::string& buffer) const override;
    };
}


#endif // myserver_POSTv1updateMessage
