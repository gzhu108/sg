#include "RestResponse.h"

using namespace sg::microreactor;


bool RestResponse::FlushToBuffer(std::string& buffer)
{
    buffer = mVersion + " " + std::to_string(mStatusCode) + " " + mStatusText + "\r\n";

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

bool RestResponse::SendWith(Connection& connection, uint32_t statusCode, const std::string& statusText, const std::vector<HttpHeader>& headers, std::shared_ptr<std::string> body, bool connectionClosed)
{
    RestResponse response;
    response.mStatusCode = statusCode;
    response.mStatusText = statusText;
    response.mHeaders.insert(response.mHeaders.end(), headers.begin(), headers.end());
    if (connectionClosed)
    {
        response.mHeaders.emplace_back(HttpHeader("Connection", "Closed"));
    }
    SetHttpBody(body, response);

    return response.Send(connection);
}
