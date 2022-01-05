#include "MessageBase.h"

using namespace microreactor;
using namespace streetgangapi;


MessageBase::MessageBase()
{
    ResponseTimeout.set(std::chrono::milliseconds(30000));
}

MessageBase::~MessageBase()
{
}
