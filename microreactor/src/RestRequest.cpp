#include "RestRequest.h"

using namespace sg::microreactor;


bool RestRequest::Write(std::string& buffer) const
{
    buffer = mMethod + " " + mUri + " " + mVersion + "\r\n";

    bool chunked = false;
    for (const auto& header : mHeaders)
    {
        buffer += header.mName + ": " + header.mValue + "\r\n";
        if (header.mName == "Transfer-Encoding" && header.mValue == "chunked")
        {
            chunked = true;
        }
    }

    if (chunked)
    {
        buffer += "\r\n";
    }
    else
    {
        if (mBody.mLength)
        {
            buffer += "Content-Length: " + std::to_string(mBody.mLength) + "\r\n";
            buffer += "\r\n" + std::string(mBody.mOffset, mBody.mLength);
        }
        else
        {
            buffer += "\r\n";
        }
    }

    return buffer.length() > 0;
}
