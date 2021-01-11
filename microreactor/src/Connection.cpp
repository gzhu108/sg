#include "Connection.h"
#include <mutex>
#include <chrono>
#include "Serializer.h"
#include "Dispatcher.h"
#include "TaskManagerSingleton.h"
#include "Reactor.h"

using namespace sg::microreactor;


static std::mutex gConnectionMutex;
static uint64_t gRequestCount;
static std::chrono::time_point<std::chrono::high_resolution_clock> gLastReportTime{std::chrono::high_resolution_clock::now()};

static void LogTps()
{
    gConnectionMutex.lock();
    gRequestCount++;
    
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
{
}

Connection::~Connection()
{
    mPrecessMessageTask->Completed.Disconnect(reinterpret_cast<uintptr_t>(this));
    mPrecessMessageTask->Cancel();
    RemoveAllReactors();
}

Connection& Connection::RegisterMessage(std::shared_ptr<Message> message, std::shared_ptr<Reactor> client)
{
    message->Client.set(client);

    auto dispatcher = Dispatcher.cref();
    if (dispatcher != nullptr)
    {
        dispatcher->RegisterMessage(message);
    }

    return *this;
}

bool Connection::Receive(std::iostream& stream)
{
    if (mReceiveBufferSize == 0)
    {
        return false;
    }

    std::vector<char> receiveBuffer(mReceiveBufferSize);
    int received = (int)Receive(&receiveBuffer[0], mReceiveBufferSize);
    if (received > 0)
    {
        stream.clear();
        stream.write(&receiveBuffer[0], received);

        if (!stream.eof() && !stream.fail() && !stream.bad())
        {
            return true;
        }
    }

    return false;
}

bool Connection::Send(std::iostream& stream)
{
    int length = (int)GetStreamSize(stream);
    if (length == 0)
    {
        return false;
    }

    std::vector<char> sendBuffer(length);
    stream.read(&sendBuffer[0], length);
    if (stream.eof() || stream.fail() || stream.bad())
    {
        return false;
    }

    return Send(&sendBuffer[0], length) > 0;
}

bool Connection::Start()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (!IsClosed())
    {
        // Push to the queue to receive connection data.
        mPrecessMessageTask = SUBMIT(std::bind(&Connection::ProcessMessage, this), "Connection::PrecessMessage");
        if (mPrecessMessageTask != nullptr)
        {
            auto taskRawPtr = mPrecessMessageTask.get();
            mPrecessMessageTask->Completed.Connect(std::bind([](Task* task)
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
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (!IsClosed())
    {
        Close();
    }

    mPrecessMessageTask->Completed.Disconnect(reinterpret_cast<uintptr_t>(this));
    mPrecessMessageTask->Cancel();

    RemoveAllReactors();
    mClosed();

    return true;
}

void Connection::ProcessMessage()
{
    // Flush the socket first
    Flush();
    
    auto dispatcher = Dispatcher.cref();
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
