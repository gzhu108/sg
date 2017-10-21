#include "MessageBase.h"

using namespace sg::microreactor;
using namespace streetgangapi;


MessageBase::MessageBase()
{
    ResponseTimeout.set(30000);
}

MessageBase::~MessageBase()
{
}
