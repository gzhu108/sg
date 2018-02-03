#pragma once
#ifndef myserver_POSTv1updateMessage
#define myserver_POSTv1updateMessage

#include "Microreactor.h"


namespace myserver
{
    class POSTv1updateMessage : public sg::microreactor::RestMessage
    {
    public:
        POSTv1updateMessage(std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~POSTv1updateMessage();

        // TODO: Add message properties here
    };
}


#endif // myserver_POSTv1updateMessage
