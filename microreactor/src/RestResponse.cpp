#include "RestResponse.h"
#include "http_parser.h"

using namespace sg::microreactor;


static int OnMessageBegin(http_parser* parser)
{
    return 0;
}

static int OnUrl(http_parser* parser, const char* start, size_t length)
{
    return 0;
}

static int OnStatus(http_parser* parser, const char* start, size_t length)
{
    auto response = static_cast<RestResponse*>(parser->data);
    response->mStatusCode = parser->status_code;
    response->mStatusText.assign(start, length);
    return 0;
}

static int OnHeaderField(http_parser* parser, const char* start, size_t length)
{
    HttpHeader header;
    header.mName.assign(start, length);

    auto response = static_cast<RestResponse*>(parser->data);
    response->mHeaders.emplace_back(header);

    return 0;
}

static int OnHeaderValue(http_parser* parser, const char* start, size_t length)
{
    auto response = static_cast<RestResponse*>(parser->data);
    response->mHeaders.back().mValue.assign(start, length);
    return 0;
}

static int OnHeadersComplete(http_parser* parser)
{
    auto response = static_cast<RestResponse*>(parser->data);
    response->mVersion = "HTTP/";
    response->mVersion += std::to_string(parser->http_major);
    response->mVersion += ".";
    response->mVersion += std::to_string(parser->http_minor);
    return 0;
}

static int OnBody(http_parser* parser, const char* start, size_t length)
{
    auto response = static_cast<RestResponse*>(parser->data);

    if (response->mChunks.empty())
    {
        response->mBody.mOffset = start;
        response->mBody.mLength = length;
    }
    else if (!response->mChunkCompleted)
    {
        response->mChunks.back().mBody.mOffset = start;
        response->mChunks.back().mBody.mLength = length;
    }

    return 0;
}

static int OnMessageComplete(http_parser* parser)
{
    return 0;
}

static int OnChunkHeader(http_parser* parser)
{
    // When on_chunk_header is called, the current chunk length is stored 
    // in parser->content_length.

    auto response = static_cast<RestResponse*>(parser->data);
    if (parser->content_length)
    {
        RestResponse chunk;
        SetHttpBody(response->mRawMessage, chunk);
        response->mChunks.emplace_back(chunk);
    }
    else
    {
        response->mChunkCompleted = true;
    }

    return 0;
}

static int OnChunkComplete(http_parser* parser)
{
    return 0;
}

/*
struct http_parser_settings {
http_cb      on_message_begin;
http_data_cb on_url;
http_data_cb on_status;
http_data_cb on_header_field;
http_data_cb on_header_value;
http_cb      on_headers_complete;
http_data_cb on_body;
http_cb      on_message_complete;
http_cb      on_chunk_header;
http_cb      on_chunk_complete;
};
*/
static http_parser_settings gRestResponseSettings =
{
    OnMessageBegin,
    OnUrl,
    OnStatus,
    OnHeaderField,
    OnHeaderValue,
    OnHeadersComplete,
    OnBody,
    OnMessageComplete,
    OnChunkHeader,
    OnChunkComplete
};

bool RestResponse::Parse(std::shared_ptr<std::string> message)
{
    mRawMessage = message;
    if (mRawMessage->empty())
    {
        return false;
    }

    LOG("HTTP raw message:\n%s\n", mRawMessage->c_str());

    http_parser parser;
    parser.data = this;
    http_parser_init(&parser, HTTP_RESPONSE);
    size_t parsed = http_parser_execute(&parser, &gRestResponseSettings, mRawMessage->c_str(), mRawMessage->length());
    if (parsed != mRawMessage->length())
    {
        return false;
    }

    return true;
}

bool RestResponse::Send(Connection& connection)
{
    std::string buffer;
    if (FlushToBuffer(buffer))
    {
        LOG("\n---------------------------------------");
        LOG("%s", buffer.c_str());
        LOG("\n---------------------------------------");

        bool result = connection.Send(buffer.data(), (int32_t)buffer.length()) == buffer.length();
        if (!result || !mBody.mLength)
        {
            return result;
        }

        for (const auto& chunk : mChunks)
        {
            std::stringstream chunkStream;
            chunkStream << std::uppercase << std::hex << chunk.mBody.mLength << "\r\n" << std::string(chunk.mBody.mOffset, chunk.mBody.mLength);
            result = connection.Send(chunkStream);
        }

        std::string terminateChunk("0\r\n\r\n");
        return connection.Send(terminateChunk.data(), (int32_t)terminateChunk.length()) == terminateChunk.length();
    }

    return false;
}

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
        buffer += "Content-Length: " + std::to_string(mBody.mLength) + "\r\n";
        if (mBody.mLength)
        {
            buffer += "\r\n" + std::string(mBody.mOffset, mBody.mLength);
        }
    }

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
