#include "RestRequestCreateWorld.h"

#ifdef new
#undef new
#endif

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace microreactor;
using namespace streetgangapi;


RestRequestCreateWorld::RestRequestCreateWorld()
{
}

RestRequestCreateWorld::~RestRequestCreateWorld()
{
}

bool RestRequestCreateWorld::Decode(std::shared_ptr<microreactor::RestRequest> request)
{
    std::string path = "/createworld";
    if (request == nullptr || request->mUri.length() < path.length())
    {
        return false;
    }

    if (request->mUri.length() > path.length())
    {
        std::string worldName = request->mUri.substr(path.length() + 1, std::string::npos);
        LOG("RestCreateWorld::CreateReactor() [WorldName=%s]", worldName.c_str());
        WorldName.set(worldName);
    }

    return true;
}
