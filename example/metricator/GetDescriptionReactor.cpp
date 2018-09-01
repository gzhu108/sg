#include "GetDescriptionReactor.h"
#include "ConfigurationSingleton.h"

using namespace sg::microreactor;
using namespace metricator;


GetDescriptionReactor::GetDescriptionReactor(std::shared_ptr<Connection> connection, std::shared_ptr<RestRequest> request)
    : RestReactor(connection, request)
{
}

GetDescriptionReactor::~GetDescriptionReactor()
{
}

bool GetDescriptionReactor::Process()
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
        std::string root = "description";
        path = root + uri;
    }

    RestFileResponse response(path);
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "application/xml"));
    return response.Send(*mConnection);
}
