#include "RestGetVersion.h"
#include "RequestGetVersionReactor.h"
#include "StreetGangRestResponseTextEncoder.h"
#include "StreetGangRestResponseJsonEncoder.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


RestGetVersion::RestGetVersion()
    : RestFactory("GET", "/version", "ContentType: text/plain, application/json")
{
}

RestGetVersion::~RestGetVersion()
{
}

std::shared_ptr<Reactor> RestGetVersion::CreateReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    if (request->mUri.length() != mPath.length())
    {
        return nullptr;
    }
    
    auto message = std::make_shared<RequestGetVersion>();
    std::shared_ptr<StreetGangResponseEncoder> streetGangResponseEncoder;

    for (const auto& header : request->mHeaders)
    {
        if (header.mName == "Accept" && header.mValue == "application/json")
        {
            streetGangResponseEncoder = std::make_shared<StreetGangRestResponseJsonEncoder>();
            break;
        }
    }
    
    if (streetGangResponseEncoder == nullptr)
    {
        // default content type is plain text
        streetGangResponseEncoder = std::make_shared<StreetGangRestResponseTextEncoder>();
    }

    auto reactor = std::make_shared<RequestGetVersionReactor>(connection, message);
    reactor->SetMessageEncoder(streetGangResponseEncoder);
    return reactor;
}
