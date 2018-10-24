#include "Connection.h"
#include "Serializer.h"
#include "Dispatcher.h"
#include "TaskManagerSingleton.h"
#include <mutex>
#include <chrono>

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

Connection::Connection(std::shared_ptr<Profile> profile)
    : mProfile(profile)
{
}

Connection::~Connection()
{
    CancelAllTasks(ReceiveTimeout.cref() + SendTimeout.cref());
}

void Connection::RegisterMessage(std::shared_ptr<Message> message, std::shared_ptr<Reactor> client)
{
    if (mProfile != nullptr && mProfile->Dispatcher.cref() != nullptr)
    {
        message->Client.set(client);
        mProfile->Dispatcher.cref()->RegisterMessage(message);
    }
}

bool Connection::Receive(std::iostream& stream)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    int32_t length = (int32_t)mReceiveBuffer.size();
    if (length == 0)
    {
        return false;
    }

    int32_t received = (int32_t)Receive(&mReceiveBuffer[0], length);
    if (received > 0)
    {
        stream.clear();
        stream.write(&mReceiveBuffer[0], received);

        if (!stream.eof() && !stream.fail() && !stream.bad())
        {
            return true;
        }
    }

    return false;
}

bool Connection::Send(std::iostream& stream)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    int32_t length = (int32_t)GetStreamSize(stream);
    if (length == 0)
    {
        return false;
    }

    mSendBuffer.resize(length);
    stream.read(&mSendBuffer[0], length);
    if (stream.eof() || stream.fail() || stream.bad())
    {
        return false;
    }

    return Send(&mSendBuffer[0], length) > 0;
}

bool Connection::Start()
{
    if (!IsClosed() && ReceiveTimeout->count())
    {
        // Push to the queue to receive connection data.
        SUBMIT_MEMBER(Connection::ReceiveMessage, "Connection::ReceiveMessage");
        return true;
    }

    return false;
}

bool Connection::Stop()
{
    if (!IsClosed())
    {
        Close();
    }

    CancelAllTasks(ReceiveTimeout.cref());
    mClosed();

    return true;
}

void Connection::ReceiveMessage()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (mProfile->Dispatcher.cref() != nullptr)
    {
        // Remove timed out messages
        mProfile->Dispatcher.cref()->RemoveTimedOutMessages(*this);

        // Receive data from the connection
        if (!IsClosed() && DataReady())
        {
            LogTps();
            mProfile->Dispatcher.cref()->Dispatch(*this);
        }
    }

    if (!IsClosed())
    {
        // Push to the queue to receive connection data.
        SUBMIT_MEMBER(Connection::ReceiveMessage, "Connection::ReceiveMessage");
    }
}

void Connection::CancelAllTasks(const std::chrono::microseconds& waitTime)
{
    uint64_t cancelCount = CANCEL_TASKS(this);

    if (waitTime.count() > 0 && cancelCount > 0)
    {
        while (GET_ACTIVE_TASK_COUNT(this) > 0)
        {
            std::this_thread::sleep_for(waitTime);
        }
    }
}
