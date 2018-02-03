#include "$serviceclassService.h"
@group_begin
#include "$method$funcReactor.h"
@group_end

using namespace sg::microreactor;
using namespace $namespace;


$serviceclassService::$serviceclassService(std::shared_ptr<Endpoint> endpoint, std::shared_ptr<Profile> profile)
    : RestService(endpoint, profile)
{
}

$serviceclassService::~$serviceclassService()
{
}

bool $serviceclassService::Initialize()
{
    @group_begin
    mRestMessageDecoder->RegisterRestReactorFactory("$method", "$path", std::bind(&$serviceclassService::Create$method$funcReactor, this, std::placeholders::_1, std::placeholders::_2));
    @group_end
    
    return RestService::Initialize();
}

@group_begin
std::shared_ptr<Reactor> $serviceclassService::Create$method$funcReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    if (request->mUri.length() < std::string("$path").length())
    {
        return nullptr;
    }

    return std::make_shared<$method$funcReactor>(connection, request);
}

@group_end
