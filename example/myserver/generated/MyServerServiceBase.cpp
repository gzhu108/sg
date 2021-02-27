#include "MyServerServiceBase.h"
#include "GETv1versionReactorBase.h"
#include "POSTv1updateReactorBase.h"


namespace myserver
{
    MyServerServiceBase::MyServerServiceBase(const std::string& hostAddress, uint16_t port)
        : RestService(hostAddress, port)
    {
    }

    MyServerServiceBase::MyServerServiceBase(std::shared_ptr<microreactor::Profile> profile)
        : RestService(profile)
    {
    }

    MyServerServiceBase::MyServerServiceBase(std::shared_ptr<microreactor::Listener> listener)
        : RestService(listener)
    {
    }

    MyServerServiceBase::~MyServerServiceBase()
    {
    }

    bool MyServerServiceBase::Initialize()
    {
        mListener->GetDispatcher<microreactor::RestDispatcher>()->RegisterResource("GET", "/v1/version", std::bind(&MyServerServiceBase::CreateGETv1versionReactor, this, std::placeholders::_1, std::placeholders::_2));
        mListener->GetDispatcher<microreactor::RestDispatcher>()->RegisterResource("POST", "/v1/update", std::bind(&MyServerServiceBase::CreatePOSTv1updateReactor, this, std::placeholders::_1, std::placeholders::_2));

        return RestService::Initialize();
    }
}
