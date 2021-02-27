#include "Message.h"

using namespace microreactor;


Message::Message()
{
}

Message::~Message()
{
}

bool Message::HasTimedOut()
{
    if (ResponseTimeout.cref() > 0)
    {
        auto responseTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - mCreationTime).count();
        if (responseTime > ResponseTimeout.cref())
        {
            return true;
        }
    }

    return false;
}

bool Message::Read(const std::string& buffer)
{
    std::stringstream stream(buffer);
    return Decode(stream);
}

bool Message::Write(std::string& buffer) const
{
    std::stringstream stream;
    if (Encode(stream))
    {
        buffer = stream.str();
        return true;
    }

    return false;
}

bool Message::Encode(std::ostream& stream) const
{
    return false;
}

bool Message::Decode(std::istream& stream)
{
    return false;
}
