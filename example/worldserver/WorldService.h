#pragma once
#ifndef worldserver_WorldService
#define worldserver_WorldService

#include "TaskManagerSingleton.h"
#include "Microservice.h"
#include "Client.h"


namespace worldserver
{
    class WorldService : public sg::microreactor::Microservice
    {
    public:
        WorldService();
        virtual ~WorldService();
        
    protected:
        virtual bool Initialize() override;
        
    protected:
        sg::microreactor::Signal<void>::SignalId mConfigurationConnectionId;
    };
}

#endif // worldserver_WorldService

