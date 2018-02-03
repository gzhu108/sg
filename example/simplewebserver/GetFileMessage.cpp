#include "GetFileMessage.h"

using namespace sg::microreactor;
using namespace simplewebserver;


GetFileMessage::GetFileMessage(std::shared_ptr<RestRequest> request)
    : RestMessage(request)
{
}

GetFileMessage::~GetFileMessage()
{
}
