#include "RequestSearchReactor.h"
#include "StringUtility.h"
#include "UdpConnection.h"

using namespace sg::microreactor;
using namespace sg::service;


RequestSearchReactor::RequestSearchReactor(Connection& connection, std::shared_ptr<RequestSearch> message)
    : MessageReactor(connection, message)
{
}

RequestSearchReactor::~RequestSearchReactor()
{
}

bool RequestSearchReactor::Process()
{
    auto udpConnection = static_cast<UdpConnection*>(&mConnection);
    LOG("peer = %s:%u", udpConnection->Source->c_str(), udpConnection->Port.cref());
    return true;
}
