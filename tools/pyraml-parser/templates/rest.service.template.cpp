#include "$serviceclassServiceBase.h"
@group_begin
#include "$method$funcReactorBase.h"
@group_end

using namespace sg::microreactor;
using namespace $namespace;


$serviceclassServiceBase::$serviceclassServiceBase(std::shared_ptr<Endpoint> endpoint, std::shared_ptr<Profile> profile)
    : RestService(endpoint, profile)
{
}

$serviceclassServiceBase::~$serviceclassServiceBase()
{
}

bool $serviceclassServiceBase::Initialize()
{
    @group_begin
    mRestMessageDecoder->RegisterRestReactorFactory("$method", "$path", std::bind(&$serviceclassServiceBase::Create$method$funcReactor, this, std::placeholders::_1, std::placeholders::_2));
    @group_end
    
    return RestService::Initialize();
}

@group_begin
// Override Create$method$funcReactor() in the derived class
std::shared_ptr<Reactor> $serviceclassServiceBase::Create$method$funcReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    return std::make_shared<$method$funcReactorBase>(connection, request);
}

@group_end
