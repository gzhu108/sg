#include "RequestByebyeReactor.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


RequestByebyeReactor::RequestByebyeReactor(Connection& connection, std::shared_ptr<RequestByebye> message)
    : MessageReactor(connection, message)
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
