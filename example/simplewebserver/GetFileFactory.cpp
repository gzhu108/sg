#include "GetFileFactory.h"
#include "GetFileReactor.h"

using namespace sg::microreactor;
using namespace simplewebserver;


GetFileFactory::GetFileFactory()
    : RestFactory("GET", "/")
{
}

GetFileFactory::~GetFileFactory()
{
}

std::shared_ptr<Reactor> GetFileFactory::CreateReactor(std::shared_ptr<RestRequest> request, Connection& connection)
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
