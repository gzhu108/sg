#include "Connection.h"
#include <mutex>
#include <chrono>
#include "Serializer.h"
#include "Dispatcher.h"
#include "TaskManagerSingleton.h"
#include "Reactor.h"

using namespace microreactor;


static std::mutex gConnectionMutex;
static uint64_t gRequestCount;
static std::chrono::time_point<std::chrono::high_resolution_clock> gLastReportTime{std::chrono::high_resolution_clock::now()};

static void LogTps()
{
    gConnectionMutex.lock();
    ++gRequestCount;
    
    std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sinceLastTime = now - gLastReportTime;
    if (sinceLastTime.count() >= 1.0)
    {
        LOG("TPS=%f", gRequestCount / sinceLastTime.count());
        gRequestCount = 0;
        gLastReportTime = now;
    }
    gConnectionMutex.unlock();
}

Connection::Connection()
    : mReceiveBufferSize(0)
    , mStopped(false)
{
}

Connection::~Connection()
{
    mProcessMessageTask->Completed.Disconnect(reinterpret_cast<uintptr_t>(this));
    mProcessMessageTask->Cancel();
    RemoveAllReactors();
}

Connection& Connection::RegisterMessage(std::shared_ptr<Message> message, std::shared_ptr<Reactor> client)
{
    message->Client(client);

    auto dispatcher = Dispatcher();
    if (dispatcher != nullptr)
    {
        dispatcher->RegisterMessage(message);
    }

    return *this;
}

bool Connection::Read(std::iostream& stream)
{
    SharedBuffer receiveBuffer = Receive();
    if (receiveBuffer == nullptr || receiveBuffer->empty())
    {
        return false;
    }

    stream.clear();
    stream.write(receiveBuffer->data(), receiveBuffer->size());
    if (!stream.eof() && !stream.fail() && !stream.bad())
    {
        return true;
    }

    return false;
}

bool Connection::Write(std::iostream& stream)
{
    int length = (int)GetStreamSize(stream);
    if (length == 0)
    {
        return false;
    }

    SharedBuffer sendBuffer = std::make_shared<Buffer>(length, '\0');
    stream.read(&sendBuffer->front(), length);
    if (stream.eof() || stream.fail() || stream.bad())
    {
        return false;
    }

    return Send(sendBuffer);
}

SharedBuffer Connection::Receive()
{
    if (mReceiveBufferSize == 0)
    {
        return nullptr;
    }

    std::vector<char> receiveBuffer(mReceiveBufferSize);
    int received = (int)Receive(&receiveBuffer[0], mReceiveBufferSize);
    if (received > 0)
    {
        return std::make_shared<Buffer>(&receiveBuffer[0], received);
    }

    return nullptr;
}

bool Connection::Send(SharedBuffer buffer)
{
    buffer.StartTimer();
    return mSendBufferQueue.Submit(buffer);
}

bool Connection::Start()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (!IsClosed())
    {
        // Push to the queue to receive connection data.
        mProcessMessageTask = SUBMIT(std::bind(&Connection::ProcessMessage, this), "Connection::PrecessMessage");
        if (mProcessMessageTask != nullptr)
        {
            auto taskRawPtr = mProcessMessageTask.get();
            mProcessMessageTask->Completed.Connect(std::bind([](Task* task)
            {
                task->Schedule();
            }, taskRawPtr), reinterpret_cast<uintptr_t>(this));

            return true;
        }
    }

    return false;
}

bool Connection::Stop()
{
    mStopped = true;

    // Send remaining messages before closing
    SendMessage();

    ScopeLock<decltype(mLock)> scopeLock(mLock);
    if (!IsClosed())
    {
        Close();
    }

    mProcessMessageTask->Completed.Disconnect(reinterpret_cast<uintptr_t>(this));
    mProcessMessageTask->Cancel();

    RemoveAllReactors();
    mClosed();

    return true;
}

void Connection::ReceiveMessage()
{
    if (mStopped)
    {
        return;
    }
    
    auto dispatcher = Dispatcher();
    if (dispatcher != nullptr)
    {
        // Remove timed out messages
        dispatcher->RemoveTimedOutMessages(*this);

        // Receive data from the connection
        if (!IsClosed())
        {
            LogTps();
            dispatcher->Dispatch(*this);
        }
    }
}

void Connection::SendMessage()
{
    // Send buffered messages
    SharedBuffer buffer = mSendBufferQueue.GetBuffer();
    while (buffer != nullptr)
    {
        if (buffer.HasTimedOut() || Send(buffer->c_str(), (int)buffer->size()) > 0)
        {
            buffer = mSendBufferQueue.GetBuffer();
        }
        else
        {
            mSendBufferQueue.PutBack(buffer);
            buffer = nullptr;
        }
    }

    // Flush the socket first
    Flush();
}

void Connection::ProcessMessage()
{
    SendMessage();
    ReceiveMessage();
}

void Connection::AddReactor(std::shared_ptr<Reactor> reactor)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (reactor != nullptr)
    {
        mActiveReactors.emplace(reactor);
        reactor->Completed.Connect([=]()
        {
            RemoveReactor(reactor);
        }, reinterpret_cast<uintptr_t>(this));
    }
}

void Connection::RemoveReactor(std::shared_ptr<Reactor> reactor)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (reactor != nullptr)
    {
        auto found = mActiveReactors.find(reactor);
        if (found != mActiveReactors.end())
        {
            reactor->Completed.Disconnect(reinterpret_cast<uintptr_t>(this));

            // The order of the calls is important
            mActiveReactors.erase(reactor);
            reactor->Stop();
        }
    }
}

void Connection::RemoveAllReactors()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    for (auto& reactor : mActiveReactors)
    {
        reactor->Completed.Disconnect(reinterpret_cast<uintptr_t>(this));
        reactor->Stop();
    }

    mActiveReactors.clear();
}
