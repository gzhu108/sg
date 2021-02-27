#include "RequestByebyeReactor.h"

using namespace microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


RequestByebyeReactor::RequestByebyeReactor(Connection& connection, std::shared_ptr<RequestByebye> message, std::shared_ptr<StreetGangResponder> responder)
    : MessageReactor(connection, message)
    , StreetGangReactor(responder)
{
}

RequestByebyeReactor::~RequestByebyeReactor()
{
}

bool RequestByebyeReactor::Process()
{
    // Cannot call Stop here, because this is within the task execution.
	//mConnection.Stop();
    return true;
}
