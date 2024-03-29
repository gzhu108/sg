#ifdef _MSC_VER
#pragma warning(disable : 4503)
#endif

#include "RestDispatcher.h"
#include "Connection.h"
#include "TaskManagerSingleton.h"
#include "StringUtility.h"

#define DEFAULT_HTTP_BUFFER_SIZE 10240

using namespace microreactor;


RestDispatcher::RestDispatcher()
{
}

RestDispatcher::~RestDispatcher()
{
}

void RestDispatcher::Dispatch(Connection& connection)
{
    auto reactor = Decode(connection);
    if (reactor != nullptr)
    {
        connection.AddReactor(reactor);

        try
        {
            reactor->Process();
            reactor->CheckComplete();
        }
        catch (...)
        {
            LOG("Exception: Failed to run Rest Reactor's Process()");
        }
    }
}

void RestDispatcher::RegisterResource(const std::string& method, const std::string& uri, Factory factory)
{
    if (factory == nullptr)
    {
        return;
    }

    std::string key = method;
    StringUtility::ToUpper(key);
    key += uri;

    RegisterReactorFactory(key, factory);
}

std::shared_ptr<Reactor> RestDispatcher::Decode(Connection& connection)
{
    auto message = connection.Receive();
    if (message == nullptr || message->empty())
    {
        return nullptr;
    }

    std::shared_ptr<RestMessage> parent = PopChunkedMessage(connection);
    std::shared_ptr<RestMessage> restMessage = restMessage->Parse(parent, message);
    if (restMessage == nullptr)
    {
        // Do not response if the message is malformed
        //SendBadMessageResponse(connection);
        return nullptr;
    }

    // Handle chunked data before Getting the API object.
    if (restMessage->mChunks.empty() || restMessage->mChunkCompleted)
    {
        for (auto& header : restMessage->mHeaders)
        {
            if (header.mName == "Content-Length")
            {
                size_t contentLength = std::strtoul(header.mValue.c_str(), nullptr, 10);
                if (restMessage->mBody.mLength > contentLength)
                {
                    restMessage->mBody.mLength = contentLength;
                }

                break;
            }
        };

        return CreateReactor(restMessage, connection);
    }

    PushChunkedMessage(connection, restMessage);
    return nullptr;
}

std::shared_ptr<Reactor> RestDispatcher::CreateReactor(std::shared_ptr<RestMessage> restMessage, Connection& connection)
{
    if (restMessage == nullptr)
    {
        return nullptr;
    }

    if (restMessage->mMessageType == RestMessage::Request)
    {
        auto restReactorFactory = GetRestReactorFactory(std::static_pointer_cast<RestRequest>(restMessage));
        if (restReactorFactory == nullptr)
        {
            SendNotFoundResponse(connection);
            return nullptr;
        }

        try
        {
            auto reactor = restReactorFactory(restMessage, connection);
            return reactor;
        }
        catch(const std::exception& e)
        {
            LOG("Exception: Failed to run rest reactor factory (%s)", e.what());
            SendBadMessageResponse(connection);
            return nullptr;
        }
    }
    else if (restMessage->mMessageType == RestMessage::Response)
    {
        auto restReactorFactory = GetRestReactorFactory(std::static_pointer_cast<RestResponse>(restMessage));
        if (restReactorFactory == nullptr)
        {
            SendNotFoundResponse(connection);
            return nullptr;
        }

        try
        {
            auto reactor = restReactorFactory(restMessage, connection);
            return reactor;
        }
        catch(const std::exception& e)
        {
            LOG("Exception: Failed to run rest reactor factory (%s)", e.what());
            SendNotFoundResponse(connection);
            return nullptr;
        }
    }

    return nullptr;
}

RestDispatcher::Factory RestDispatcher::GetRestReactorFactory(std::shared_ptr<RestRequest> restRequest)
{
    if (restRequest == nullptr || restRequest->mUri.empty() || mReactorFactoryTable.empty())
    {
        return nullptr;
    }

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
        LOG("\n%.*s", (int)chunk->mBody.mLength, chunk->mBody.mOffset);
        LOG("\n---- Chunk ----");
    }
    LOG("\n+++++++++++++++++++++++++++++++++++++++");

    std::string key = restRequest->mMethod;
    StringUtility::ToUpper(key);
    key += restRequest->mUri;

    auto low = mReactorFactoryTable.lower_bound(key);
    auto high = mReactorFactoryTable.upper_bound(key);

    if (low == high)
    {
        // No exact match found
        if (high == mReactorFactoryTable.begin())
        {
            // the URI is not within the range in the API table.
            return nullptr;
        }

        // Move the lower bound back one position
        low--;

        if (key.empty() && low->first.empty())
        {
            return low->second;
        }
        else if (key.empty() || low->first.empty())
        {
            return nullptr;
        }

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

RestDispatcher::Factory RestDispatcher::GetRestReactorFactory(std::shared_ptr<RestResponse> restResponse)
{
    if (restResponse == nullptr || mReactorFactoryTable.empty())
    {
        return nullptr;
    }

    LOG("\n+++++++++++++++++++++++++++++++++++++++");
    LOG("%d %s %s", restResponse->mStatusCode, restResponse->mStatusText.c_str(), restResponse->mVersion.c_str());
    for (auto& header : restResponse->mHeaders)
    {
        LOG("%s: %s", header.mName.c_str(), header.mValue.c_str());
    }
    LOG("\n%.*s", (int)restResponse->mBody.mLength, restResponse->mBody.mOffset);
    for (auto& chunk : restResponse->mChunks)
    {
        LOG("\n++++ Chunk ++++");
        LOG("\n%.*s", (int)chunk->mBody.mLength, chunk->mBody.mOffset);
        LOG("\n---- Chunk ----");
    }
    LOG("\n+++++++++++++++++++++++++++++++++++++++");

    // Use an empty key for response
    std::string key;
    auto found = mReactorFactoryTable.find(key);
    if (found == mReactorFactoryTable.end())
    {
        return nullptr;
    }

    return found->second;
}

void RestDispatcher::PushChunkedMessage(microreactor::Connection& connection, std::shared_ptr<RestMessage> restMessage)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    mChunkedMessageStore[reinterpret_cast<uintptr_t>(&connection)] = restMessage;
}

std::shared_ptr<RestMessage> RestDispatcher::PopChunkedMessage(microreactor::Connection& connection)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    std::shared_ptr<RestMessage> restMessage;

    auto found = mChunkedMessageStore.find(reinterpret_cast<uintptr_t>(&connection));
    if (found != mChunkedMessageStore.end())
    {
        restMessage = found->second;
        mChunkedMessageStore.erase(found);
    }

    return restMessage;
}

void RestDispatcher::SendBadMessageResponse(Connection& connection)
{
    RestResponse::SendErrorWith(connection, 400, "Bad Message");
}

void RestDispatcher::SendNotFoundResponse(Connection& connection)
{
    RestResponse::SendErrorWith(connection, 404, "Not Found");
}
