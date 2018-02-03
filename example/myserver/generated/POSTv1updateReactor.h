#pragma once
#ifndef myserver_POSTv1updateReactor
#define myserver_POSTv1updateReactor

#include "Microreactor.h"
#include "POSTv1updateMessage.h"


namespace myserver
{
    class POSTv1updateReactor : public sg::microreactor::RestReactor<POSTv1updateMessage>
    {
    public:
        POSTv1updateReactor(sg::microreactor::Connection& connection, std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~POSTv1updateReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // myserver_POSTv1updateReactor
