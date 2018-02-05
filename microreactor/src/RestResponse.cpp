#include "RestResponse.h"

using namespace sg::microreactor;


bool RestResponse::Send(Connection& connection)
{
    std::string buffer;
    if (FlushToBuffer(buffer))
    {
        LOG("\n---------------------------------------");
        LOG("%s", buffer.c_str());
        LOG("\n---------------------------------------");

        return connection.Send(buffer.data(), (int32_t)buffer.length()) == buffer.length();
    }

    return false;
}

bool RestResponse::FlushToBuffer(std::string& buffer)
{
    buffer = mVersion + " " + std::to_string(mStatusCode) + " " + mStatusText + "\r\n";
    for (const auto& header : mHeaders)
    {
        buffer += header.mName + ": " + header.mValue + "\r\n";
    }

    if (mBody.length() > 0)
    {
        buffer += "Content-Length: " + std::to_string(mBody.length()) + "\r\n";
        buffer += "\r\n" + mBody;
    }

    buffer += "\r\n";
    return buffer.length() > 0;
}

bool RestResponse::FlushToStream(std::ostream& stream)
{
    std::string buffer;
    if (FlushToBuffer(buffer))
    {
        stream.write(buffer.data(), buffer.length());
        return !stream.eof() && !stream.fail() && !stream.bad();
    }

    return false;
}

bool RestResponse::SendWith(Connection& connection, uint32_t statusCode, const std::string& statusText, const std::vector<HttpHeader>& headers, const std::string& body, bool connectionClosed)
{
    RestResponse response;
    response.mStatusCode = statusCode;
    response.mStatusText = statusText;
    response.mHeaders.insert(response.mHeaders.end(), headers.begin(), headers.end());
    if (connectionClosed)
    {
        response.mHeaders.emplace_back(HttpHeader("Connection", "Closed"));
    }
    response.mBody = body;
    return response.Send(connection);
}
