#include "RestRequestByebye.h"

#ifdef new
#undef new
#endif

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace microreactor;
using namespace streetgangapi;


RestRequestByebye::RestRequestByebye()
{
}

RestRequestByebye::~RestRequestByebye()
{
}

bool RestRequestByebye::Decode(std::shared_ptr<microreactor::RestRequest> request)
{
    if (request == nullptr)
    {
        return false;
    }

    return true;
}
