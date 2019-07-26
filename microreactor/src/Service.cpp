#include "Service.h"
#include "NetworkUtility.h"
#include "Serializer.h"

using namespace sg::microreactor;


Service::Service()
{
}

Service::Service(std::shared_ptr<Endpoint> endpoint)
    : mEndpoint(endpoint)
{
}

Service::~Service()
{
}

bool Service::Start()
{
    if (Initialize() && mEndpoint != nullptr)
    {
        if (mEndpoint->Start())
        {
            LOG("Service started on %s", mEndpoint->Name->c_str());

            // Connection to the ConnectonMade signal
            mEndpoint->ConnectionMade.Connect(std::bind(&Service::OnConnectionMade, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));
            return true;
        }
    }

    return false;
}

bool Service::Stop()
{
    if (mEndpoint != nullptr)
    {
        if (!mEndpoint->Stop())
        {
            return false;
        }

        // Disconnect the ConnectonMade signal
        mEndpoint->ConnectionMade.Disconnect(reinterpret_cast<uintptr_t>(this));
        mEndpoint = nullptr;
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
    if (mEndpoint == nullptr || buffer == nullptr || length <= 0)
    {
        return -1;
    }

    std::set<std::shared_ptr<Connection>> activeConnections;
    if (!mEndpoint->GetAllConnections(activeConnections))
    {
        // No connection to send data
        return -1;
    }

    int64_t errorCount = 0;
    for (auto& connection : activeConnections)
    {
        if (connection->Send(buffer, length) <= 0)
        {
            errorCount++;
        }
    }

    return errorCount;
}

bool Service::Initialize()
{
    return mEndpoint != nullptr;
}

void Service::OnConnectionMade(const std::shared_ptr<Connection>& connection)
{
    std::string hostName = connection->Name.cref();
    std::string connectionName = std::string("[") + connection->GetPeerAddress() + "]:" + std::to_string(connection->GetPeerPort());

    connection->Closed.Connect([=]()
    {
        LOG("Disconnected %s -> %s", hostName.c_str(), connectionName.c_str());
    });

    LOG("Connection accepted %s -> %s", hostName.c_str(), connectionName.c_str());
}
