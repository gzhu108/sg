#include "GETv1versionMessage.h"

#ifdef new
#undef new
#endif

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace sg::microreactor;
using namespace myserver;


GETv1versionMessage::GETv1versionMessage(std::shared_ptr<RestRequest> request)
    : RestMessage(request)
{
    // TODO: parse HTTP request here
}

GETv1versionMessage::~GETv1versionMessage()
{
}
