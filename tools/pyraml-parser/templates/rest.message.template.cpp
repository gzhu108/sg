#include "$method$funcMessage.h"

#ifdef new
#undef new
#endif

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace sg::microreactor;
using namespace $namespace;


$method$funcMessage::$method$funcMessage(std::shared_ptr<RestRequest> request)
    : RestMessage(request)
{
    // TODO: parse HTTP request here
}

$method$funcMessage::~$method$funcMessage()
{
}
