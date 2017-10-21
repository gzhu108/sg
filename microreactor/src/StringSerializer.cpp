#include "StringSerializer.h"
#include "Endianness.h"

using namespace sg::microreactor;


StringSerializer::StringSerializer()
{
}

StringSerializer::~StringSerializer()
{
}

bool StringSerializer::ReadCount(std::istream& /*stream*/, uint16_t& /*count*/)
{
    return true;
}

bool StringSerializer::WriteCount(uint16_t /*count*/, std::ostream& /*stream*/)
{
    return true;
}

