#include "RequestGetVersion.h"

using namespace sg::microreactor;
using namespace streetgangapi;


RequestGetVersion::RequestGetVersion()
{
    Id.set((int32_t)ID::GetVersionRequest);
}

RequestGetVersion::~RequestGetVersion()
{
}
