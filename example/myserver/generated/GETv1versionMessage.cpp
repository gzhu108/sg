#include "GETv1versionMessage.h"

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
