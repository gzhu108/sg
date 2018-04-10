#include "Reactor.h"

using namespace sg::microreactor;


Reactor::Reactor(std::shared_ptr<Connection> connection)
    : mConnection(connection)
{
}

Reactor::~Reactor()
{
}
