#include "ResponseGetVersionReactor.h"
#include <thread>
#include "Microreactor.h"
#include "ResponseGetVersion.h"
#include "StreetGangApi.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


ResponseGetVersionReactor::ResponseGetVersionReactor(Connection& connection, std::shared_ptr<ResponseGetVersion> message)
    : MessageReactor(connection, message)
{
}

ResponseGetVersionReactor::~ResponseGetVersionReactor()
{
}

bool ResponseGetVersionReactor::Process()
{
    auto latency = mParentMessage == nullptr ? 0 : std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - mParentMessage->GetRequestTime()).count();
    auto& version = InputMessage()->Version.cref();

    LOG("[TrackId=%s] [Latency=" FMT_INT64 "] [Result=%d] ResponseGetVersionReactor() [version=%s]",
        InputMessage()->TrackId->c_str(),
        latency,
        InputMessage()->Result.cref(),
        version.c_str());

    SendNextRequest();
    return true;
}

void ResponseGetVersionReactor::SendNextRequest()
{
    static int worldNumber = 0;
    std::stringstream ss;
    ss << "World(" << worldNumber << ")";
    worldNumber++;

    StreetGangApi api(mConnection, mMessageEncoder);
    if (!api.CreateWorld(ss.str()))
    {
        LOG("%s Failed to create a new world", ss.str().c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //SUBMIT(std::bind(&ResponseGetVersionReactor::SendNextRequest, this), shared_from_this(), this, "ResponseGetVersionReactor::SendNextRequest");
    }
}
