#include "GetFileReactor.h"

using namespace sg::microreactor;
using namespace simplewebserver;


GetFileReactor::GetFileReactor(Connection& connection, std::shared_ptr<RestRequest> request)
    : Reactor(connection)
    , mRequest(request)
{
}

GetFileReactor::~GetFileReactor()
{
}

bool GetFileReactor::Process()
{
    // trim '/' at the end of URI
    std::string uri = mRequest->mUri;
    if (uri.back() == '/')
    {
        uri.resize(uri.length() - 1);
    }

    std::string path;
    if (!uri.empty())
    {
        std::string root = "webroot";
        auto configuration = mConnection.GetProfile()->Configuration.cref();
        path = configuration->GetValue("WebRoot", root) + uri;
    }

    RestFileResponse response(path);
    return response.Send(mConnection);
}
