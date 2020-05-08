#include "POSTv1updateRequestContent.h"

using namespace sg::microreactor;
using namespace myserver;


POSTv1updateRequestContent::POSTv1updateRequestContent()
{
}

POSTv1updateRequestContent::~POSTv1updateRequestContent()
{
}

bool POSTv1updateRequestContent::Encode(nlohmann::json& serializer) const
{
    serializer["Name"] = Name.cref();
    serializer["Age"] = Age.cref();
    return true;
}

bool POSTv1updateRequestContent::Decode(const nlohmann::json& serializer)
{
    serializer["Name"].get_to(Name.ref());
    serializer["Age"].get_to(Age.ref());
    return true;
}

bool POSTv1updateRequestContent::Write(std::string& buffer) const
{
    nlohmann::json serializer;
    Encode(serializer);
    buffer = serializer.dump();
    return true;
}
