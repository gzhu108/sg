#include "MSearchResponseReactor.h"
#include "RestResponse.h"
#include "StringUtility.h"

using namespace sg::microreactor;
using namespace sg::service;


MSearchResponseReactor::MSearchResponseReactor(std::shared_ptr<Connection> connection, std::shared_ptr<RestResponse> response)
    : RestReactor(connection, response)
{
}

MSearchResponseReactor::~MSearchResponseReactor()
{
}

bool MSearchResponseReactor::Process()
{
    auto response = InputMessage();
    if (response == nullptr || response->mStatusCode != 200)
    {
        return false;
    }

    LOG("M-SEARCH response received from %s:%u", mConnection->GetPeerAddress().c_str(), mConnection->GetPeerPort());

    ServiceDescription description;
    for (const auto& header : response->mHeaders)
    {
        if (header.mName == "CACHE-CONTROL")
        {
            //description.NotifyMaxAge.set(std::atoi(header.mValue.c_str));
        }
        else if (header.mName == "LOCATION" || header.mName == "AL")
        {
            description.Location.set(header.mValue);
        }
        else if (header.mName == "ST")
        {
            description.ServiceType.set(header.mValue);
        }
        else if (header.mName == "SERVER")
        {
            description.ServerInfo.set(header.mValue);
        }
        else if (header.mName == "USN")
        {
            description.Usn.set(header.mValue);
        }
        else if (header.mName == "DATE")
        {
            description.Date.set(header.mValue);
        }
    }

    if (description.ServiceType.cref() == ServiceType.cref())
    {
        mServiceFound(description);
    }

    return true;
}
