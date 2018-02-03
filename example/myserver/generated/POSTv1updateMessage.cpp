#include "POSTv1updateMessage.h"

using namespace sg::microreactor;
using namespace myserver;


POSTv1updateMessage::POSTv1updateMessage(std::shared_ptr<RestRequest> request)
    : RestMessage(request)
{
    // TODO: parse HTTP request here
}

POSTv1updateMessage::~POSTv1updateMessage()
{
}
