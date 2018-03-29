#include "StreetGangReactor.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


StreetGangReactor::StreetGangReactor(std::shared_ptr<StreetGangResponder> responder)
    : mResponder(responder)
{
}

StreetGangReactor::~StreetGangReactor()
{
}
