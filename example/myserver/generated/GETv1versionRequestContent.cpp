#include "GETv1versionRequestContent.h"

using namespace microreactor;
using namespace myserver;


GETv1versionRequestContent::GETv1versionRequestContent()
{
}

GETv1versionRequestContent::~GETv1versionRequestContent()
{
}

bool GETv1versionRequestContent::Encode(nlohmann::json& serializer) const
{
    serializer["Version"] = Version.cref();
    return true;
}

bool GETv1versionRequestContent::Decode(const nlohmann::json& serializer)
{
    return true;
}

bool GETv1versionRequestContent::Write(std::string& buffer) const
{
    nlohmann::json serializer;
    Encode(serializer);
    buffer = serializer.dump();
    return true;
}
