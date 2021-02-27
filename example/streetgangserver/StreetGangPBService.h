#pragma once
#ifndef streetgangserver_StreetGangPBService
#define streetgangserver_StreetGangPBService

#include "TaskManagerSingleton.h"
#include "Service.h"
#include "Client.h"
#include "MessageBase.h"


namespace streetgangserver
{
    class StreetGangPBService : public microreactor::Service
    {
    public:
        StreetGangPBService();
        virtual ~StreetGangPBService();
        
    protected:
        virtual bool Start() override;
        virtual bool Stop() override;
        virtual bool Initialize() override;
        
        void SetResponseTime(std::shared_ptr<streetgangapi::MessageBase> response);
        void OnTaskPreprocess(const microreactor::TaskPtr& task);
        void OnTaskPostprocess(const microreactor::TaskPtr& task);
        
    protected:
        microreactor::Signal<void>::SignalId mConfigurationConnectionId;

        std::map<void*, std::chrono::time_point<std::chrono::high_resolution_clock>> mTaskProcessTime;
        std::shared_ptr<microreactor::Client> mDiscoveryClient;
        double mTaskLatencyThreshold;
    };
}

#endif // streetgangserver_StreetGangPBService

