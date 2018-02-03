#pragma once
#ifndef $namespace_$serviceclassService
#define $namespace_$serviceclassService

#include "Microreactor.h"


namespace $namespace
{
    class $serviceclassService : public sg::microreactor::RestService
    {
    public:
        $serviceclassService(std::shared_ptr<sg::microreactor::Endpoint> endpoint, std::shared_ptr<sg::microreactor::Profile> profile);
        virtual ~$serviceclassService();

    public:
        virtual bool Initialize() override;

    protected:
        @group_begin
        virtual std::shared_ptr<sg::microreactor::Reactor> Create$method$funcReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection);
        @group_end
    };
}


#endif // $namespace_$serviceclassService
