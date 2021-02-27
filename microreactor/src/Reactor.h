#pragma once
#ifndef microreactor_Reactor
#define microreactor_Reactor

#include "Parkable.h"
#include "Message.h"
#include "Connection.h"


namespace microreactor
{
    class Reactor : public Parkable<std::string>
    {
    public:
        explicit Reactor(Connection& connection, std::shared_ptr<Message> input = nullptr);
        virtual ~Reactor();

        Signal<void>& Completed = mCompleted;

    public:
        virtual bool Process() { return false; }
        virtual bool ProcessError(std::shared_ptr<Message> errorMessage) { return false; }
        virtual bool ProcessTimeout(std::shared_ptr<Message> timedOutMessage) { return false; }
        virtual std::shared_ptr<Message> Input() { return mInput; }

        virtual void Stop();

        virtual void CheckComplete();
        virtual TaskPtr AddTask(TaskPtr task);
        virtual void RemoveTask(TaskPtr task);
        virtual void RemoveAllTasks();

    protected:
        Emittable<void> mCompleted;
        std::recursive_mutex mLock;
        std::set<TaskPtr> mActiveTasks;
        Connection& mConnection;
        std::shared_ptr<Message> mInput;
    };
}


#endif // microreactor_Reactor
