#pragma once
#ifndef sg_microreactor_Host
#define sg_microreactor_Host

#include <chrono>
#include "Connection.h"


namespace sg { namespace microreactor
{
    class Host
    {
    public:
        explicit Host(std::shared_ptr<Profile> profile);
        virtual ~Host();

        Signal<std::shared_ptr<Connection>>& ConnectionMade = mConnectionMade;

        PROPERTY(Name, std::string);
        PROPERTY(ListenTimeout, std::chrono::milliseconds, std::chrono::milliseconds(1));
        PROPERTY(ReceiveTimeout, std::chrono::milliseconds, std::chrono::milliseconds(1));
        PROPERTY(SendTimeout, std::chrono::milliseconds, std::chrono::milliseconds(100));
        
    public:
        virtual bool Start();
        virtual bool Stop();
        virtual bool IsClosed() = 0;

    protected:
        virtual bool Close() = 0;
        virtual void AcceptConnection();
        virtual void CancelAllTasks(const std::chrono::microseconds& waitTime);
        virtual void RemoveClosedConnections();

        virtual std::shared_ptr<Connection> Listen(const std::chrono::milliseconds& timeout) = 0;

    protected:
        Emittable<std::shared_ptr<Connection>> mConnectionMade;
        std::shared_ptr<Profile> mProfile;

        static std::map<std::shared_ptr<Connection>, Host*> mActiveConnections;
        static std::recursive_mutex mActiveConnectionsLock;
    };
}}


#endif // sg_microreactor_Host
