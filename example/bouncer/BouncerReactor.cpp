#include "BouncerReactor.h"
#include <chrono>
#include "Serializer.h"
#include "TaskManagerSingleton.h"

using namespace microreactor;
using namespace bouncer;


BouncerReactor::BouncerReactor(Connection& connection, std::shared_ptr<Connection> target, std::istream& stream)
    : Reactor(connection)
    , mTarget(target)
{
    auto length = GetStreamSize(stream);
    if (length > 0)
    {
        mData.resize(length);
        stream.read(&mData[0], length);
    }
}

BouncerReactor::~BouncerReactor()
{
}

bool BouncerReactor::Process()
{
    if (mData.empty() || mTarget == nullptr)
    {
        return false;
    }

    LOG("%s: Received " FMT_UINT64 " bytes", mConnection.Name->c_str(), mData.size());

    auto bytesSent = mTarget->Send(&mData[0], (int32_t)mData.size());
    if (bytesSent != mData.size())
    {
        return false;
    }

    LOG("%s: Sent " FMT_UINT64 " bytes", mTarget->Name->c_str(), bytesSent);
    return ReceiveTarget();
}

bool BouncerReactor::ReceiveTarget()
{
    std::vector<char> response;
    response.resize(10240);

    if (!mTarget->IsClosed())
    {
        auto bytesReceived = mTarget->Receive(&response[0], (int32_t)response.size());
        LOG("%s: Recieved " FMT_UINT64 " bytes", mTarget->Name->c_str(), bytesReceived);
        LOG("++++++++++++++++\n%.*s\n-------------------", bytesReceived, &response[0]);

        if (bytesReceived)
        {
            auto bytesSent = mConnection.Send(&response[0], (int32_t)bytesReceived);
            if (bytesSent != bytesReceived)
            {
                return false;
            }

            LOG("%s: Sent " FMT_UINT64 " bytes", mConnection.Name->c_str(), bytesSent);
        }

        SUBMIT_MEMBER(BouncerReactor::ReceiveTarget, "BouncerReactor::ReceiveTarget");
        return true;
    }

    return false;
}
