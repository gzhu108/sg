#pragma once
#ifndef streetgangserver_StreetGangBinaryService
#define streetgangserver_StreetGangBinaryService

#include "TaskManagerSingleton.h"
#include "Microservice.h"
#include "Client.h"
#include "MessageBase.h"


namespace streetgangserver
{
    class StreetGangBinaryService : public sg::microreactor::Microservice
    {
    public:
        StreetGangBinaryService();
        virtual ~StreetGangBinaryService();
        
    protected:
        virtual bool Initialize() override;
        
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

#endif // streetgangserver_StreetGangBinaryService

