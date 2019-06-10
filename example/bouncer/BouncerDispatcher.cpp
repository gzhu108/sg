#include "BouncerDispatcher.h"
#include "BouncerReactor.h"
#include "NetworkUtility.h"
#include "Serializer.h"

using namespace sg::microreactor;
using namespace bouncer;


BouncerDispatcher::BouncerDispatcher()
{
}

BouncerDispatcher::~BouncerDispatcher()
{
}

std::shared_ptr<sg::microreactor::Reactor> BouncerDispatcher::Decode(std::istream& stream, Connection& connection)
{
    if (GetStreamSize(stream) == 0)
    {
        return nullptr;
    }

    std::string protocol = Protocol.cref();
    std::string targetName = TargetName.cref();
    uint16_t targetPort = TargetPort.cref();

    auto targetDispatcher = std::make_shared<Dispatcher>();
    targetDispatcher->Protocol.set(protocol);
    targetDispatcher->Address.set(targetName);
    targetDispatcher->Port.set(targetPort);

    std::shared_ptr<Connection> target = NetworkUtility::CreateConnection(targetDispatcher);
    return std::make_shared<BouncerReactor>(connection, target, stream);
}
