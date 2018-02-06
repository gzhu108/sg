#pragma once
#ifndef myserver_POSTv1updateMessage
#define myserver_POSTv1updateMessage

#include "RestMessage.h"


namespace myserver
{
    class POSTv1updateMessage : public sg::microreactor::RestMessage
    {
    public:
        POSTv1updateMessage(std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~POSTv1updateMessage();

        PROPERTY(Name, std::string, "");
        PROPERTY(Age, std::string, "18");
    };
}


#endif // myserver_POSTv1updateMessage
