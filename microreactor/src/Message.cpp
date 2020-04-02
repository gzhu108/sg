#include "Message.h"

using namespace sg::microreactor;


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

bool Message::Encode(std::ostream& stream) const
{
    std::string buffer;
    if (Write(buffer))
    {
        stream.write(buffer.data(), buffer.length());
        return !stream.eof() && !stream.fail() && !stream.bad();
    }

    return false;
}

bool Message::Decode(std::istream& stream)
{
    std::string buffer;
    stream >> buffer;
    return Read(buffer);
}
