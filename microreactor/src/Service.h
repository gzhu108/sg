#pragma once
#ifndef microreactor_Service
#define microreactor_Service

#include "Listener.h"
#include "NetworkUtility.h"


namespace microreactor
{
    class Service
    {
    public:
        Service();
        explicit Service(std::shared_ptr<Listener> listener);
        virtual ~Service();

    public:
        virtual bool Start();
        virtual bool Stop();
        virtual void Restart();

        virtual int64_t SendAllConnections(std::iostream& stream);
        virtual int64_t SendAllConnections(const char* buffer, int32_t length);

    protected:
        virtual bool Initialize();
        virtual void OnConnectionMade(const std::shared_ptr<Connection>& connection);

    protected:
        std::shared_ptr<Listener> mListener;
    };
}

#endif // microreactor_Service
