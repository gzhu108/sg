#include "GetFileApi.h"
#include "GetFileReactor.h"

using namespace sg::microreactor;
using namespace simplewebserver;


GetFileApi::GetFileApi()
    : RestApi("GET", "/")
{
}

GetFileApi::~GetFileApi()
{
}

std::shared_ptr<Reactor> GetFileApi::CreateReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    if (request->mUri.length() < mPath.length())
    {
        return nullptr;
    }

    // No need to parse JSON
    auto input = std::make_shared<Message>();

    auto reactor = std::make_shared<GetFileReactor>(connection, request, input);
    return reactor;
}
