#pragma once
#ifndef streetgangserver_StreetGangPBService
#define streetgangserver_StreetGangPBService

#include "TaskManagerSingleton.h"
#include "Microservice.h"
#include "Client.h"
#include "MessageBase.h"


namespace streetgangserver
{
    class StreetGangPBService : public sg::microreactor::Microservice
    {
    public:
        StreetGangPBService();
        virtual ~StreetGangPBService();
        
    public:
        virtual bool Start() override;
        virtual bool Stop() override;

    protected:
        bool CreatePBListener();
        
        void OnConnectionMade(const std::shared_ptr<const sg::microreactor::Connection>& connection);

        void SetResponseTime(std::shared_ptr<streetgangapi::MessageBase> response);
        void OnTaskPreprocess(const sg::microreactor::TaskPtr& task);
        void OnTaskPostprocess(const sg::microreactor::TaskPtr& task);
        
    protected:
        sg::microreactor::Signal<void>::SignalId mConfigurationConnectionId;

        std::map<void*, std::chrono::time_point<std::chrono::high_resolution_clock>> mTaskProcessTime;
        std::shared_ptr<sg::microreactor::Client> mDiscoveryClient;
        double mTaskLatencyThreshold;
    };
}

#endif // streetgangserver_StreetGangPBService

