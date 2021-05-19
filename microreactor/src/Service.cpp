#include "Service.h"
#include "NetworkUtility.h"
#include "Serializer.h"

using namespace microreactor;


Service::Service()
{
}

Service::Service(std::shared_ptr<Listener> listener)
    : mListener(listener)
{
}

Service::~Service()
{
}

bool Service::Start()
{
    if (Initialize() && mListener != nullptr)
    {
        if (mListener->Start())
        {
            LOG("Service started on %s", mListener->Name->c_str());

            // Connection to the ConnectonMade signal
            mListener->ConnectionMade.Connect(std::bind(&Service::OnConnectionMade, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));
            return true;
        }
    }

    return false;
}

bool Service::Stop()
{
    if (mListener != nullptr)
    {
        if (!mListener->Stop())
        {
            return false;
        }

        // Disconnect the ConnectonMade signal
        mListener->ConnectionMade.Disconnect(reinterpret_cast<uintptr_t>(this));
        mListener = nullptr;
        return true;
    }

    return false;
}

void Service::Restart()
{
    Stop();
    Start();
}

int64_t Service::SendAllConnections(std::iostream& stream)
{
    int32_t length = (int32_t)GetStreamSize(stream);
    if (length == 0)
    {
        return -1;
    }

    std::vector<char> sendBuffer(length);
    stream.read(&sendBuffer[0], length);
    if (stream.eof() || stream.fail() || stream.bad())
    {
        return -1;
    }

    return SendAllConnections(&sendBuffer[0], length);
}

int64_t Service::SendAllConnections(const char* buffer, int32_t length)
{
    if (mListener == nullptr || buffer == nullptr || length <= 0)
    {
        return -1;
    }

    std::set<std::shared_ptr<Connection>> activeConnections;
    if (!mListener->GetAllConnections(activeConnections))
    {
        // No connection to send data
        return -1;
    }

    int64_t errorCount = 0;
    for (auto& connection : activeConnections)
    {
        if (connection->Send(buffer, length) <= 0)
        {
            ++errorCount;
        }
    }

    return errorCount;
}

bool Service::Initialize()
{
    return mListener != nullptr;
}

void Service::OnConnectionMade(const std::shared_ptr<Connection>& connection)
{
    std::string hostName = connection->Name();
    std::string connectionName = std::string("[") + connection->GetPeerAddress() + "]:" + std::to_string(connection->GetPeerPort());

    connection->Closed.Connect([=]()
    {
        LOG("Disconnected %s -> %s", hostName.c_str(), connectionName.c_str());
    });

    LOG("Connection accepted %s -> %s", hostName.c_str(), connectionName.c_str());
}
