#include "RequestCreateWorld.h"

using namespace sg::microreactor;
using namespace streetgangapi;


RequestCreateWorld::RequestCreateWorld()
{
    Id.set((int)ID::CreateWorldRequest);
}

RequestCreateWorld::~RequestCreateWorld()
{
}
