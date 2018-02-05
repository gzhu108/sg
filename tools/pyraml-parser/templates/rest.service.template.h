#pragma once
#ifndef $namespace_$serviceclassServiceBase
#define $namespace_$serviceclassServiceBase

#include "Microreactor.h"


namespace $namespace
{
    class $serviceclassServiceBase : public sg::microreactor::RestService
    {
    public:
        $serviceclassServiceBase(std::shared_ptr<sg::microreactor::Endpoint> endpoint, std::shared_ptr<sg::microreactor::Profile> profile);
        virtual ~$serviceclassServiceBase();

    public:
        virtual bool Initialize() override;

    protected:
        @group_begin
        virtual std::shared_ptr<sg::microreactor::Reactor> Create$method$funcReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection);
        @group_end
    };
}


#endif // $namespace_$serviceclassServiceBase
