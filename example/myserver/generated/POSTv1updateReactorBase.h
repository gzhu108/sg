#pragma once
#ifndef myserver_POSTv1updateReactorBase
#define myserver_POSTv1updateReactorBase

#include "RestReactor.h"


namespace myserver
{
    class POSTv1updateReactorBase : public sg::microreactor::RestReactor<sg::microreactor::RestRequest>
    {
    public:
        POSTv1updateReactorBase(sg::microreactor::Connection& connection, std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~POSTv1updateReactorBase();

    public:
        // Override Process() in the derived class
        virtual bool Process() override;
    };
}


#endif // myserver_POSTv1updateReactorBase
