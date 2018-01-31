#include "$serviceclassService.h"
@incl_creator_begin
#include "$method$funcReactor.h"
@incl_creator_end

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
    @init_creator_begin
    mRestMessageDecoder->RegisterRestReactorFactory("$method", "$path", std::bind(&$serviceclassService::Create$method$funcReactor, this, std::placeholders::_1, std::placeholders::_2));
    @init_creator_end
    
    return RestService::Initialize();
}

@impl_creator_begin
std::shared_ptr<Reactor> $serviceclassService::Create$method$funcReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    if (request->mUri.length() < std::string("$path").length())
    {
        return nullptr;
    }

    // No need to parse JSON
    auto input = std::make_shared<Message>();

    auto reactor = std::make_shared<$method$funcReactor>(connection, request, input);
    return reactor;
}

@impl_creator_end
