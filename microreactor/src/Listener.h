#pragma once
#ifndef microreactor_Listener
#define microreactor_Listener

#include <chrono>
#include "Connection.h"
#include "Task.h"


namespace microreactor
{
    class Listener
    {
    public:
        Listener();
        virtual ~Listener();

        Signal<std::shared_ptr<Connection>>& ConnectionMade = mConnectionMade;

        PROPERTY(Name, std::string);
        PROPERTY(SendTimeout, std::chrono::milliseconds, std::chrono::milliseconds(100));
        PROPERTY(Dispatcher, std::shared_ptr<microreactor::Dispatcher>);
        
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

        virtual std::shared_ptr<Connection> Listen() = 0;

    protected:
        std::recursive_mutex mLock;
        Emittable<std::shared_ptr<Connection>> mConnectionMade;
        std::set<std::shared_ptr<Connection>> mActiveConnections;
        TaskPtr mAcceptTask;
    };
}


#endif // microreactor_Listener
