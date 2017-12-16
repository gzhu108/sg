#include "RestCreateWorld.h"
#include "RequestCreateWorldReactor.h"
#include "StreetGangRestResponseTextEncoder.h"
#include "StreetGangRestResponseJsonEncoder.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


RestCreateWorld::RestCreateWorld()
    : RestFactory("POST", "/createworld", "ContentType: text/plain, application/json")
{
}

RestCreateWorld::~RestCreateWorld()
{
}

std::shared_ptr<Reactor> RestCreateWorld::CreateReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    if (request->mUri.length() < mPath.length())
    {
        return nullptr;
    }

    auto message = std::make_shared<RequestCreateWorld>();
    if (request->mUri.length() > mPath.length())
    {
        std::string worldName = request->mUri.substr(mPath.length() + 1, std::string::npos);
        LOG("RestCreateWorld::CreateReactor() [WorldName=%s]", worldName.c_str());
        message->WorldName.set(worldName);
    }

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

    auto reactor = std::make_shared<RequestCreateWorldReactor>(connection, message);
    reactor->SetMessageEncoder(streetGangResponseEncoder);

    return reactor;
}
