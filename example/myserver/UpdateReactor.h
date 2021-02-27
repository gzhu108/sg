#pragma once
#ifndef myserver_UpdateReactor
#define myserver_UpdateReactor

#include "RestRequest.h"
#include "generated/POSTv1updateReactorBase.h"


namespace myserver
{
    class UpdateReactor : public POSTv1updateReactorBase
    {
    public:
        UpdateReactor(microreactor::Connection& connection, std::shared_ptr<microreactor::RestRequest> request);
        virtual ~UpdateReactor();

    public:
        // Override Process() in the derived class
        virtual bool Process() override;
    };
}


#endif // myserver_UpdateReactor
