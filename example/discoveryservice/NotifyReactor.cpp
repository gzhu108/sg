#include "NotifyReactor.h"
#include "RestResponse.h"
#include "StringUtility.h"

using namespace microreactor;
using namespace sg::service;


NotifyReactor::NotifyReactor(Connection& connection, std::shared_ptr<RestRequest> request)
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
        return false;
    }

    std::string uri = request->mUri;
    if (uri != "*")
    {
        return false;
    }

    LOG("NOTIFY received from %s:%u", mConnection.GetPeerAddress().c_str(), mConnection.GetPeerPort());

    std::string host;
    std::string cacheControl;
    std::string location;
    std::string nts;
    std::string server;
    Uuid usn;
    std::string nt;

    for (const auto& header : request->mHeaders)
    {
        if (header.mName == "HOST")
        {
            host = header.mValue;
        }
        else if (header.mName == "CACHE-CONTROL")
        {
            cacheControl = header.mValue;
        }
        else if (header.mName == "LOCATION" || header.mName == "AL")
        {
            location = header.mValue;
        }
        else if (header.mName == "NTS")
        {
            nts = header.mValue;
        }
        else if (header.mName == "SERVER")
        {
            server = header.mValue;
        }
        else if (header.mName == "USN")
        {
            usn = Uuid(header.mValue);
        }
        else if (header.mName == "NT")
        {
            nt = header.mValue;
        }
    }

    if (nt == ServiceType.cref())
    {
        if (nts == "ssdp:alive")
        {
            UnicastMSearch(mConnection.GetPeerAddress(), mConnection.GetPeerPort());
        }
        if (nts == "ssdp:byebye")
        {
            mByebye(usn);
        }
    }

    return true;
}

void NotifyReactor::UnicastMSearch(const std::string& unicastAddress, uint16_t port)
{
    mConnection.SetPeerAddress(unicastAddress).SetPeerPort(port);

    RestRequest request;
    request.mMethod = "M-SEARCH";
    request.mUri = "*";
    request.mVersion = "HTTP/1.1";
    request.mHeaders.emplace_back(HttpHeader("HOST", unicastAddress + ":" + std::to_string(port)));
    request.mHeaders.emplace_back(HttpHeader("MAN", "ssdp:discover"));
    request.mHeaders.emplace_back(HttpHeader("MX", "0"));
    request.mHeaders.emplace_back(HttpHeader("ST", ServiceType.cref()));

    std::string buffer;
    if (request.Write(buffer))
    {
        mConnection.Send(buffer.data(), buffer.length());
    }
}
