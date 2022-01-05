#include "WorldMessage.h"

using namespace microreactor;
using namespace worldapi;


WorldMessage::WorldMessage()
{
    ResponseTimeout.set(std::chrono::milliseconds(30000));
}

WorldMessage::~WorldMessage()
{
}
