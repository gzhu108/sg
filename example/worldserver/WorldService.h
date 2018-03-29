#pragma once
#ifndef worldserver_WorldService
#define worldserver_WorldService

#include "TaskManagerSingleton.h"
#include "Microservice.h"
#include "Client.h"
#include "MessageBase.h"


namespace worldserver
{
    class WorldService : public sg::microreactor::Microservice
    {
    public:
        WorldService();
        virtual ~WorldService();
        
    public:
        virtual bool Start() override;
        virtual bool Stop() override;

    protected:
        bool CreateBinaryListener();        
        void OnConnectionMade(const std::shared_ptr<const sg::microreactor::Connection>& connection);
        
    protected:
        sg::microreactor::Signal<void>::SignalId mConfigurationConnectionId;
    };
}

#endif // worldserver_WorldService

