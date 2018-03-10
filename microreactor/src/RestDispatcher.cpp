#ifdef _MSC_VER
#pragma warning(disable : 4503)
#endif

#include "RestDispatcher.h"
#include "Connection.h"
#include "TaskManagerSingleton.h"
#include "RestResponse.h"
#include "StringUtility.h"

#define DEFAULT_HTTP_BUFFER_SIZE 10240

using namespace sg::microreactor;


RestDispatcher::RestDispatcher()
{
}

RestDispatcher::~RestDispatcher()
{
}

void RestDispatcher::Dispatch(Connection& connection)
{
    auto reactor = Decode(connection);
    if (reactor != nullptr && InitializeReactor(*reactor))
    {
        // Submit shared_ptr reactor process to task queue so it's reference counted.
        SUBMIT(std::bind(&Reactor::Process, reactor), reactor, &connection, "Reactor::Process");
    }
}

void RestDispatcher::RegisterRestReactorFactory(const std::string& method, const std::string& uri, Factory factory)
{
    if (factory == nullptr)
    {
        return;
    }

    std::string key = method;
    StringUtility::ToUpper(key);
    key += uri;

    RegisterMessageReactorFactory(key, factory);
}

std::shared_ptr<Reactor> RestDispatcher::Decode(Connection& connection)
{
    auto message = std::make_shared<std::string>();
    message->resize(DEFAULT_HTTP_BUFFER_SIZE);

    // Receive data from the connection
    uint64_t length = connection.Receive(&message->at(0), (int32_t)message->length());
    message->resize((size_t)length);

    if (message->empty())
    {
        return nullptr;
    }

    std::shared_ptr<RestRequest> restRequest = PopChunkedRequest(connection);
    if (restRequest == nullptr)
    {
        restRequest = std::make_shared<RestRequest>();
    }

    if (restRequest->Parse(message))
    {
        LOG("\n+++++++++++++++++++++++++++++++++++++++");
        LOG("%s %s %s", restRequest->mMethod.c_str(), restRequest->mUri.c_str(), restRequest->mVersion.c_str());
        for (auto& header : restRequest->mHeaders)
        {
            LOG("%s: %s", header.mName.c_str(), header.mValue.c_str());
        }
        LOG("\n%.*s", (int)restRequest->mBody.mLength, restRequest->mBody.mOffset);
        for (auto& chunk : restRequest->mChunks)
        {
            LOG("\n++++ Chunk ++++");
            LOG("\n%.*s", (int)chunk.mLength, chunk.mOffset);
            LOG("\n---- Chunk ----");
        }
        LOG("\n+++++++++++++++++++++++++++++++++++++++");
    }
    else
    {
        RestResponse::SendErrorWith(connection, 400, "Bad Request");
        return nullptr;
    }

    // Handle chunked data before Getting the API object.
    if (restRequest->mChunks.empty() || restRequest->mChunkCompleted)
    {
        for (auto& header : restRequest->mHeaders)
        {
            if (header.mName == "Content-Length")
            {
                size_t contentLength = std::strtoul(header.mValue.c_str(), nullptr, 10);
                if (restRequest->mBody.mLength > contentLength)
                {
                    restRequest->mBody.mLength = contentLength;
                }

                break;
            }
        };

        auto restReactorFactory = GetRestReactorFactory(restRequest);
        if (restReactorFactory == nullptr)
        {
            RestResponse::SendErrorWith(connection, 404, "Not Found");
            return nullptr;
        }

        auto reactor = restReactorFactory(restRequest, connection);
        if (reactor == nullptr)
        {
            RestResponse::SendErrorWith(connection, 400, "Bad Request");
        }

        return reactor;
    }

    PushChunkedRequest(connection, restRequest);
    return nullptr;
}

RestDispatcher::Factory RestDispatcher::GetRestReactorFactory(std::shared_ptr<RestRequest> restRequest)
{
    if (restRequest == nullptr || restRequest->mUri.empty() || mMessageReactorFactoryTable.empty())
    {
        return nullptr;
    }

    std::string key = restRequest->mMethod;
    StringUtility::ToUpper(key);
    key += restRequest->mUri;

    auto low = mMessageReactorFactoryTable.lower_bound(key);
    auto high = mMessageReactorFactoryTable.upper_bound(key);

    if (low == high)
    {
        // No exact match found
        if (high == mMessageReactorFactoryTable.begin())
        {
            // the URI is not within the range in the API table.
            return nullptr;
        }

        // Move the lower bound back one position
        low--;

        // Check if the lower bound key is a substring of the URL
        if (key.find(low->first) == 0 && (low->first.back() == '/' || key[low->first.length()] == '/'))
        {
            return low->second;
        }
    }
    else
    {
        // Exact match found
        return low->second;
    }

    return nullptr;
}

void RestDispatcher::PushChunkedRequest(sg::microreactor::Connection& connection, std::shared_ptr<RestRequest> restRequest)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    mChunkedRequestStore[reinterpret_cast<uintptr_t>(&connection)] = restRequest;
}

std::shared_ptr<RestRequest> RestDispatcher::PopChunkedRequest(sg::microreactor::Connection& connection)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    std::shared_ptr<RestRequest> restRequest;

    auto found = mChunkedRequestStore.find(reinterpret_cast<uintptr_t>(&connection));
    if (found != mChunkedRequestStore.end())
    {
        restRequest = found->second;
        mChunkedRequestStore.erase(found);
    }

    return restRequest;
}
