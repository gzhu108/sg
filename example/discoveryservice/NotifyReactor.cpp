#include "NotifyReactor.h"
#include "RestResponse.h"
#include "StringUtility.h"

using namespace sg::microreactor;
using namespace sg::service;


NotifyReactor::NotifyReactor(std::shared_ptr<Connection> connection, std::shared_ptr<RestRequest> request)
    : RestReactor(connection, request)
{
}

NotifyReactor::~NotifyReactor()
{
}

bool NotifyReactor::Process()
{
    auto request = InputMessage();
    if (request == nullptr)
    {
        LOG("Invalid request [ReqId=%s]\n", InputMessage()->TrackId.cref().c_str());
        return false;
    }

    std::string uri = request->mUri;
    if (uri != "*")
    {
        return false;
    }

    LOG("M-SEARCH received from %s:%u", mConnection->GetPeerName().c_str(), mConnection->GetPeerPort());

    std::string man;
    std::string mx;
    std::string st;
    for (const auto& header : request->mHeaders)
    {
        if (header.mName == "MAN")
        {
            man = header.mValue;
        }
        else if (header.mName == "MX")
        {
            mx = header.mValue;
        }
        else if (header.mName == "ST")
        {
            st = header.mValue;
        }
    }

    if (man == "ssdp:discover" && st == ServiceType.cref())
    {
        uint64_t waitTimeSeconds = std::atoi(mx.c_str());
        if (waitTimeSeconds > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(waitTimeSeconds * 1000));
        }

        RestResponse response;
        response.mHeaders.emplace_back(HttpHeader("CACHE-CONTROL", std::string("max-age = ") + std::to_string(NotifyMaxAge.cref())));
        response.mHeaders.emplace_back(HttpHeader("AL", Location.cref()));
        response.mHeaders.emplace_back(HttpHeader("SERVER", ServerInfo.cref()));
        response.mHeaders.emplace_back(HttpHeader("USN", Usn.cref()));
        response.mHeaders.emplace_back(HttpHeader("ST", ServiceType.cref()));
        response.mHeaders.emplace_back(HttpHeader("DATE", StringUtility::GetHttpTimeString()));

        return response.Send(*mConnection);
    }

    return true;
}