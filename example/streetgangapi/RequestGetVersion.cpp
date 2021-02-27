#include "RequestGetVersion.h"

using namespace microreactor;
using namespace streetgangapi;


RequestGetVersion::RequestGetVersion()
{
    Id.set((int32_t)ID::GetVersionRequest);
}

RequestGetVersion::~RequestGetVersion()
{
}
