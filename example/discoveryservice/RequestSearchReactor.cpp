#include "RequestSearchReactor.h"
#include "StringUtility.h"
#include "UdpConnection.h"

using namespace sg::microreactor;
using namespace sg::service;


RequestSearchReactor::RequestSearchReactor(std::shared_ptr<Connection> connection, std::shared_ptr<RequestSearch> message)
    : MessageReactor(connection, message)
{
}

RequestSearchReactor::~RequestSearchReactor()
{
}

bool RequestSearchReactor::Process()
{
    auto udpConnection = std::static_pointer_cast<UdpConnection>(mConnection);
    LOG("peer = %s:%u", udpConnection->Source->c_str(), udpConnection->Port.cref());
    return true;
}
