#include "MessageBase.h"

using namespace microreactor;
using namespace streetgangapi;


MessageBase::MessageBase()
{
    ResponseTimeout.set(30000);
}

MessageBase::~MessageBase()
{
}
