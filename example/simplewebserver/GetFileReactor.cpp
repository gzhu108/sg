#include "GetFileReactor.h"
#include "ConfigurationSingleton.h"

using namespace sg::microreactor;
using namespace simplewebserver;


GetFileReactor::GetFileReactor(Connection& connection, std::shared_ptr<RestRequest> request)
    : RestReactor(connection, request)
{
}

GetFileReactor::~GetFileReactor()
{
}

bool GetFileReactor::Process()
{
    if (Request() == nullptr)
    {
        LOG("Invalid request [ReqId=%s]\n", InputMessage()->TrackId.cref().c_str());
        return false;
    }

    // trim '/' at the end of URI
    std::string uri = Request()->mUri;
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
    return response.Send(mConnection);
}
