#include "RestFileResponse.h"
#include "Serializer.h"
#include "StringUtility.h"

using namespace sg::microreactor;


static const uint64_t CHUNK_SIZE = 1024;
//static const uint64_t CHUNK_SIZE = 100 * 1024 * 1024;


RestFileResponse::RestFileResponse(const std::string& path)
{
    mFileStream.open(path, std::ios_base::binary);
    if (mFileStream.is_open())
    {
        mVersion = "HTTP/1.1";
        mStatusCode = 200;
        mStatusText = "OK";
        mHeaders.emplace_back(HttpHeader("Date", StringUtility::GetHttpTimeString()));

        uint64_t fileSize = GetStreamSize(mFileStream);
        if (fileSize < CHUNK_SIZE)
        {
            auto message = std::make_shared<std::string>();
            message->resize((size_t)fileSize);
            mFileStream.read(&(*message)[0], fileSize);

            SetHttpBody(message, *this);
            mBody.mOffset = mRawMessage->data();
            mBody.mLength = mRawMessage->length();
        }
        else
        {
            mHeaders.emplace_back(HttpHeader("Transfer-Encoding", "chunked"));
        }
    }
    else
    {
        mVersion = "HTTP/1.1";
        mStatusCode = 404;
        mStatusText = "Not Found";
        mHeaders.emplace_back(HttpHeader("Connection", "Closed"));
        mHeaders.emplace_back(HttpHeader("Content-Type", "text/plain"));
        SetHttpBody("404 Not Found", *this);
    }
}

RestFileResponse::~RestFileResponse()
{
    mFileStream.close();
}

bool RestFileResponse::Send(Connection& connection)
{
    std::string buffer;
    if (Write(buffer))
    {
        bool result = connection.Send(buffer.data(), (int32_t)buffer.length()) == buffer.length();
        if (!result || !mFileStream.is_open() || mBody.mLength)
        {
            return result;
        }

        uint64_t fileSize = GetStreamSize(mFileStream);
        uint64_t chunkSize = std::min(fileSize, CHUNK_SIZE);
        std::string chunk;
        
        while (chunkSize)
        {
            std::stringstream chunkSizeStream;
            chunkSizeStream << std::uppercase << std::hex << chunkSize << "\r\n";

            chunk.assign(chunkSizeStream.str());
            size_t offset = chunk.length();
            chunk.resize(offset + (size_t)chunkSize + 2);

            mFileStream.read(&chunk[offset], chunkSize);
            chunk[offset + (size_t)chunkSize] = '\r';
            chunk[offset + (size_t)chunkSize + 1] = '\n';

            result = connection.Send(chunk.data(), (int32_t)chunk.length()) == chunk.length();
            if (!result || mFileStream.eof() || mFileStream.fail() || mFileStream.bad())
            {
                break;
            }

            fileSize -= chunkSize;
            chunkSize = std::min(fileSize, CHUNK_SIZE);
        }

        std::string terminateChunk("0\r\n\r\n");
        return connection.Send(terminateChunk.data(), (int32_t)terminateChunk.length()) == terminateChunk.length();
    }

    return false;
}
