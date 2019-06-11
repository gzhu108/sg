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
        Endpoint();
        virtual ~Endpoint();

        Signal<std::shared_ptr<Connection>>& ConnectionMade = mConnectionMade;

        PROPERTY(Name, std::string);
        PROPERTY(ListenTimeout, std::chrono::milliseconds, std::chrono::milliseconds(1));
        PROPERTY(ReceiveTimeout, std::chrono::milliseconds, std::chrono::milliseconds(1));
        PROPERTY(SendTimeout, std::chrono::milliseconds, std::chrono::milliseconds(100));
        PROPERTY(Dispatcher, std::shared_ptr<sg::microreactor::Dispatcher>);
        
    public:
        template<typename T>
        std::shared_ptr<T> GetDispatcher()
        {
            return std::static_pointer_cast<T>(Dispatcher.get());
        }

        virtual bool Start();
        virtual bool Stop();
        virtual bool IsClosed() = 0;

        virtual bool GetAllConnections(std::set<std::shared_ptr<Connection>>& connections);

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
        TaskPtr mAcceptTask;
    };
}}


#endif // sg_microreactor_Endpoint
