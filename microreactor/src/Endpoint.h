#pragma once
#ifndef sg_microreactor_Endpoint
#define sg_microreactor_Endpoint

#include <chrono>
#include "Connection.h"
#include "Task.h"


namespace sg { namespace microreactor
{
    class Endpoint
    {
    public:
        explicit Endpoint(std::shared_ptr<Profile> profile);
        virtual ~Endpoint();

        Signal<std::shared_ptr<Connection>>& ConnectionMade = mConnectionMade;

        PROPERTY(Name, std::string);
        PROPERTY(ListenTimeout, std::chrono::milliseconds, std::chrono::milliseconds(1));
        PROPERTY(ReceiveTimeout, std::chrono::milliseconds, std::chrono::milliseconds(1));
        PROPERTY(SendTimeout, std::chrono::milliseconds, std::chrono::milliseconds(100));
        
    public:
        virtual bool Start();
        virtual bool Stop();
        virtual bool IsClosed() = 0;

        virtual std::shared_ptr<Profile> GetProfile() { return mProfile; }

    protected:
        virtual bool Close() = 0;
        virtual void AcceptConnection();

        virtual void AddConnection(std::shared_ptr<Connection> connection);
        virtual void RemoveConnection(std::shared_ptr<Connection> connection);
        virtual void CloseAllConnections();

        virtual std::shared_ptr<Connection> Listen(const std::chrono::milliseconds& timeout) = 0;

    protected:
        std::recursive_mutex mLock;
        Emittable<std::shared_ptr<Connection>> mConnectionMade;
        std::set<std::shared_ptr<Connection>> mActiveConnections;
        std::shared_ptr<Profile> mProfile;
        TaskPtr mAcceptTask;
    };
}}


#endif // sg_microreactor_Endpoint
