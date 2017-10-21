#include "Service.h"

using namespace sg::microreactor;


Service::Service()
{
}

Service::~Service()
{
}

void Service::Restart()
{
    Stop();
    Start();
}
