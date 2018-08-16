#include "MSearchReactor.h"

using namespace sg::microreactor;
using namespace sg::service;


MSearchReactor::MSearchReactor(std::shared_ptr<Connection> connection, std::shared_ptr<RestRequest> request)
    : RestReactor(connection, request)
{
}

MSearchReactor::~MSearchReactor()
{
}

bool MSearchReactor::Process()
{
    if (Request() == nullptr)
    {
        LOG("Invalid request [ReqId=%s]\n", InputMessage()->TrackId.cref().c_str());
        return false;
    }

    std::string uri = Request()->mUri;
    if (uri != "*")
    {
        return false;
    }

    LOG("M-SEARCH received from %s:%u", mConnection->GetPeerName().c_str(), mConnection->GetPeerPort());

    return true;
}
