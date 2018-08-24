#include "GetFileReactor.h"
#include "ConfigurationSingleton.h"

using namespace sg::microreactor;
using namespace simplewebserver;


GetFileReactor::GetFileReactor(std::shared_ptr<Connection> connection, std::shared_ptr<GetFileMessage> request)
    : RestReactor(connection, request)
{
}

GetFileReactor::~GetFileReactor()
{
}

bool GetFileReactor::Process()
{
    auto request = InputMessage();
    if (request == nullptr)
    {
        LOG("Invalid request [ReqId=%s]\n", request->TrackId.cref().c_str());
        return false;
    }

    // trim '/' at the end of URI
    std::string uri = request->mUri;
    if (uri.back() == '/')
    {
        uri.resize(uri.length() - 1);
    }

    std::string path;
    if (!uri.empty())
    {
        std::string root = "webroot";
        auto configuration = ConfigurationSingleton::GetConfiguration();
        path = configuration->GetValue("WebRoot", root) + uri;
    }

    RestFileResponse response(path);
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "image/jpg"));
    return response.Send(*mConnection);
}
