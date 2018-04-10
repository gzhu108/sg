#include "BouncerDispatcher.h"
#include "BouncerProfile.h"
#include "BouncerReactor.h"
#include "NetworkUtility.h"
#include "Serializer.h"

using namespace sg::microreactor;
using namespace bouncer;


BouncerDecoder::BouncerDecoder()
{
}

BouncerDecoder::~BouncerDecoder()
{
}

std::shared_ptr<sg::microreactor::Reactor> BouncerDecoder::Decode(std::istream& stream, Connection& connection)
{
    if (GetStreamSize(stream) == 0)
    {
        return nullptr;
    }

    auto bouncerProfile = std::static_pointer_cast<BouncerProfile>(connection.GetProfile());
    std::string protocol = bouncerProfile->Protocol.cref();
    std::string targetName = bouncerProfile->TargetName.cref();
    uint16_t targetPort = bouncerProfile->TargetPort.cref();

    auto targetProfile = std::make_shared<Profile>();
    targetProfile->Protocol.set(protocol);
    targetProfile->Address.set(targetName);
    targetProfile->Port.set(targetPort);

    std::shared_ptr<Connection> target = NetworkUtility::CreateConnection(targetProfile);
    return std::make_shared<BouncerReactor>(std::static_pointer_cast<Connection>(connection.shared_from_this()), target, stream);
}
