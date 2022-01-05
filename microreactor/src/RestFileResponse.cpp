#include "RestFileResponse.h"
#include "Serializer.h"
#include "StringUtility.h"

using namespace microreactor;


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
    SharedBuffer buffer = std::make_shared<Buffer>();
    if (Write(*buffer))
    {
        bool result = connection.Send(buffer);
        if (!result || !mFileStream.is_open() || mBody.mLength)
        {
            return result;
        }

        uint64_t fileSize = GetStreamSize(mFileStream);
        uint64_t chunkSize = std::min(fileSize, CHUNK_SIZE);
        
        while (chunkSize)
        {
            std::stringstream chunkSizeStream;
            chunkSizeStream << std::uppercase << std::hex << chunkSize << "\r\n";

            SharedBuffer chunk = std::make_shared<Buffer>(chunkSizeStream.str());
            size_t offset = chunk->size();
            chunk->resize(offset + (size_t)chunkSize + 2);

            mFileStream.read(&(*chunk)[offset], chunkSize);
            (*chunk)[offset + (size_t)chunkSize] = '\r';
            (*chunk)[offset + (size_t)chunkSize + 1] = '\n';

            result = connection.Send(chunk);
            if (!result || mFileStream.eof() || mFileStream.fail() || mFileStream.bad())
            {
                break;
            }

            fileSize -= chunkSize;
            chunkSize = std::min(fileSize, CHUNK_SIZE);
        }

        SharedBuffer terminateChunk = std::make_shared<Buffer>("0\r\n\r\n");
        return connection.Send(terminateChunk);
    }

    return false;
}
