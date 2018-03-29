#include "WorldMessage.h"

using namespace sg::microreactor;
using namespace worldapi;


WorldMessage::WorldMessage()
{
    ResponseTimeout.set(30000);
}

WorldMessage::~WorldMessage()
{
}
