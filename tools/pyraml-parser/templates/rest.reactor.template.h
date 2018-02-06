#pragma once
#ifndef $namespace_$method$funcReactorBase
#define $namespace_$method$funcReactorBase

#include "RestReactor.h"
#include "$method$funcMessage.h"


namespace $namespace
{
    class $method$funcReactorBase : public sg::microreactor::RestReactor<$method$funcMessage>
    {
    public:
        $method$funcReactorBase(sg::microreactor::Connection& connection, std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~$method$funcReactorBase();

    public:
        // Override Process() in the derived class
        virtual bool Process() override;
    };
}


#endif // $namespace_$method$funcReactorBase
