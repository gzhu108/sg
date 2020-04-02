#include "RestMessage.h"
#include "http_parser.h"
#include "UriCodec.h"
#include "RestRequest.h"
#include "RestResponse.h"

using namespace sg::microreactor;


static int OnMessageBegin(http_parser* parser)
{
    return 0;
}

static int OnUrl(http_parser* parser, const char* start, size_t length)
{
    auto request = static_cast<RestRequest*>(parser->data);
    if (request == nullptr)
    {
        request = new RestRequest();
    }

    parser->data = static_cast<RestMessage*>(request);
    request->mMethod.assign(http_method_str((http_method)parser->method));
    request->mUri.assign(start, length);
    return UriDecode(request->mUri) ? 0 : -1;
}

static int OnStatus(http_parser* parser, const char* start, size_t length)
{
    auto response = static_cast<RestResponse*>(parser->data);
    if (response == nullptr)
    {
        response = new RestResponse();
    }

    parser->data = static_cast<RestMessage*>(response);
    response->mStatusCode = parser->status_code;
    response->mStatusText.assign(start, length);
    return 0;
}

static int OnHeaderField(http_parser* parser, const char* start, size_t length)
{
    HttpHeader header;
    header.mName.assign(start, length);

    auto message = static_cast<RestMessage*>(parser->data);
    if (message == nullptr)
    {
        return 0;
    }

    message->mHeaders.emplace_back(header);

    return 0;
}

static int OnHeaderValue(http_parser* parser, const char* start, size_t length)
{
    auto message = static_cast<RestMessage*>(parser->data);
    if (message == nullptr)
    {
        return 0;
    }

    message->mHeaders.back().mValue.assign(start, length);
    return 0;
}

static int OnHeadersComplete(http_parser* parser)
{
    auto message = static_cast<RestMessage*>(parser->data);
    if (message == nullptr)
    {
        return 0;
    }

    message->mVersion = "HTTP/";
    message->mVersion += std::to_string(parser->http_major);
    message->mVersion += ".";
    message->mVersion += std::to_string(parser->http_minor);
    return 0;
}

static int OnBody(http_parser* parser, const char* start, size_t length)
{
    auto message = static_cast<RestMessage*>(parser->data);
    if (message == nullptr)
    {
        return 0;
    }

    if (message->mChunks.empty())
    {
        message->mBody.mOffset = start;
        message->mBody.mLength = length;
    }
    else if (!message->mChunkCompleted)
    {
        message->mChunks.back()->mBody.mOffset = start;
        message->mChunks.back()->mBody.mLength = length;
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

    auto message = static_cast<RestMessage*>(parser->data);
    if (message == nullptr)
    {
        return 0;
    }

    if (parser->content_length)
    {
        auto chunk = message->CreateMessage();
        SetHttpBody(message->mRawMessage, *chunk);
        message->mChunks.emplace_back(chunk);
    }
    else
    {
        message->mChunkCompleted = true;
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
static http_parser_settings gRestMessageSettings =
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

RestMessage::RestMessage(const RestMessageType& messageType)
    : mMessageType(messageType)
{
}

RestMessage::~RestMessage()
{
}

std::shared_ptr<RestMessage> RestMessage::Parse(std::shared_ptr<RestMessage> parent, std::shared_ptr<std::string> buffer)
{
    if (buffer->empty())
    {
        return nullptr;
    }

    LOG("HTTP raw message:\n%s\n", buffer->c_str());
    if (parent != nullptr)
    {
        parent->mRawMessage = buffer;
    }

    http_parser parser;
    parser.data = parent.get();
    http_parser_init(&parser, HTTP_BOTH);
    size_t parsed = http_parser_execute(&parser, &gRestMessageSettings, buffer->c_str(), buffer->length());
    if (parsed != buffer->length())
    {
        return nullptr;
    }

    auto restMessage = parent;
    if (restMessage == nullptr)
    {
        restMessage = std::shared_ptr<RestMessage>((RestMessage*)parser.data);
        if (restMessage != nullptr)
        {
            restMessage->mRawMessage = buffer;
        }
    }

    return restMessage;
}

bool RestMessage::Send(Connection& connection)
{
    std::string buffer;
    if (Write(buffer))
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
            chunkStream << std::uppercase << std::hex << chunk->mBody.mLength << "\r\n" << std::string(chunk->mBody.mOffset, chunk->mBody.mLength);
            result = connection.Send(chunkStream);
        }

        std::string terminateChunk("0\r\n\r\n");
        return connection.Send(terminateChunk.data(), (int32_t)terminateChunk.length()) == terminateChunk.length();
    }

    return false;
}

bool RestMessage::Send(Connection& connection, const Message& message)
{
    std::string buffer;
    if (message.Write(buffer))
    {
        SetHttpBody(buffer, *this);
        return Send(connection);
    }

    return false;
}
