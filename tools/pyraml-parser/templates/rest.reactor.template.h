#pragma once
#ifndef $namespace_$method$funcReactor
#define $namespace_$method$funcReactor

#include "Microreactor.h"
#include "$method$funcMessage.h"


namespace $namespace
{
    class $method$funcReactor : public sg::microreactor::RestReactor<$method$funcMessage>
    {
    public:
        $method$funcReactor(sg::microreactor::Connection& connection, std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~$method$funcReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // $namespace_$method$funcReactor
