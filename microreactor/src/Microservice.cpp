#include "Microservice.h"
#include "NetworkUtility.h"

using namespace sg::microreactor;


Microservice::Microservice()
{
}

Microservice::Microservice(std::shared_ptr<Profile> profile)
    : mProfile(profile)
{
}

Microservice::Microservice(std::shared_ptr<Endpoint> endpoint, std::shared_ptr<Profile> profile)
    : mProfile(profile)
    , mListener(std::make_shared<Listener>())
{
    mListener->Initialize(endpoint);
}

Microservice::~Microservice()
{
    Stop();
}

bool Microservice::Start()
{
    if (Initialize() && mListener != nullptr)
    {
        if (mListener->Start())
        {
            // Connection to the ConnectonMade signal
            auto endpoint = mListener->GetEndpoint();
            if (endpoint != nullptr)
            {
                endpoint->ConnectionMade.Connect(std::bind(&Microservice::OnConnectionMade, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));
                return true;
            }
        }
    }

    return false;
}

bool Microservice::Stop()
{
    bool result = true;

    if (mListener != nullptr)
    {
        if (!mListener->Stop())
        {
            result = false;
        }

        // Disconnect the ConnectonMade signal
        if (mListener->GetEndpoint() != nullptr)
        {
            mListener->GetEndpoint()->ConnectionMade.Disconnect(reinterpret_cast<uintptr_t>(this));
        }

        mListener = nullptr;
    }

    return result;
}

bool Microservice::Initialize()
{
    if (mListener != nullptr)
    {
        // Already initialized
        return true;
    }

    std::shared_ptr<Endpoint> endpoint = NetworkUtility::CreateEndpoint(mProfile);
    if (endpoint == nullptr)
    {
        return false;
    }

    mListener = std::make_shared<Listener>();
    return mListener->Initialize(endpoint);
}

void Microservice::OnConnectionMade(const std::shared_ptr<Connection>& connection)
{
    std::string hostName = connection->Name.cref();
    std::string connectionName = std::string("[") + connection->GetPeerName() + "]:" + std::to_string(connection->GetPeerPort());

    connection->Closed.Connect([=]()
    {
        LOG("Disconnected %s -> %s", hostName.c_str(), connectionName.c_str());
    });

    LOG("Connection accepted %s -> %s", hostName.c_str(), connectionName.c_str());
}
