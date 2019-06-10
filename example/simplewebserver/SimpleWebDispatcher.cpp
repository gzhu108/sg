#include "SimpleWebDispatcher.h"
#include "GetFileReactor.h"

using namespace sg::microreactor;
using namespace simplewebserver;


SimpleWebDispatcher::SimpleWebDispatcher()
{
    RegisterRestReactorFactory("GET", "/", std::bind(&SimpleWebDispatcher::CreateGetFileReactor, this, std::placeholders::_1, std::placeholders::_2));
}

SimpleWebDispatcher::~SimpleWebDispatcher()
{
}

std::shared_ptr<Reactor> SimpleWebDispatcher::CreateGetFileReactor(std::shared_ptr<RestMessage> message, Connection& connection)
{
    auto request = std::static_pointer_cast<RestRequest>(message);
    if (request->mUri.length() < std::string("/").length())
    {
        return nullptr;
    }

    auto reactor = std::make_shared<GetFileReactor>(connection, request);
    return reactor;
}
