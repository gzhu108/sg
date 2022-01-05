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

    SharedBuffer buffer = std::make_shared<Buffer>(&mData[0], (int32_t)mData.size());
    auto bytesSent = mTarget->Send(buffer);
    if (!mTarget->Send(buffer))
    {
        return false;
    }

    LOG("%s: Sent " FMT_UINT64 " bytes", mTarget->Name->c_str(), bytesSent);
    return ReceiveTarget();
}

bool BouncerReactor::ReceiveTarget()
{
    if (!mTarget->IsClosed())
    {
        SharedBuffer response = mTarget->Receive();
        if (response == nullptr || response->empty())
        {
            return false;
        }

        LOG("%s: Recieved " FMT_UINT64 " bytes", mTarget->Name->c_str(), response->size());
        LOG("++++++++++++++++\n%.*s\n-------------------", response->size(), response->c_str());

        if (!mConnection.Send(response))
        {
            return false;
        }

        LOG("%s: Sent " FMT_UINT64 " bytes", mConnection.Name->c_str(), response->size());

        SUBMIT_MEMBER(BouncerReactor::ReceiveTarget, "BouncerReactor::ReceiveTarget");
        return true;
    }

    return false;
}
