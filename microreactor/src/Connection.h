#pragma once
#ifndef sg_microreactor_Connection
#define sg_microreactor_Connection

#include <chrono>
#include <set>
#include <mutex>
#include "Parkable.h"
#include "Dispatcher.h"
#include "Message.h"
#include "Task.h"


namespace sg { namespace microreactor
{
    class Reactor;

    class Connection
    {
    public:
        explicit Connection();
        virtual ~Connection();
        
        Signal<void>& Closed = mClosed;

        PROPERTY(Name, std::string);
        PROPERTY(ReceiveTimeout, std::chrono::milliseconds, std::chrono::milliseconds(1));
        PROPERTY(SendTimeout, std::chrono::milliseconds, std::chrono::milliseconds(100));
        PROPERTY(Dispatcher, std::shared_ptr<sg::microreactor::Dispatcher>);
        
    public:
        template<typename T>
        std::shared_ptr<T> GetDispatcher()
        {
            return std::static_pointer_cast<T>(Dispatcher.get());
        }

        virtual void RegisterMessage(std::shared_ptr<Message> message, std::shared_ptr<Reactor> client);

        virtual std::string GetPeerAddress() const = 0;
        virtual uint16_t GetPeerPort() const = 0;

        virtual void SetPeerAddress(const std::string& peerAddress) {};
        virtual void SetPeerPort(uint16_t peerPort) {};

        virtual bool Receive(std::iostream& stream);
        virtual bool Send(std::iostream& stream);

        virtual bool DataReady() = 0;
        virtual uint64_t Receive(char* buffer, int32_t length) = 0;
        virtual uint64_t Send(const char* buffer, int32_t length) = 0;
        
        virtual bool Start();
        virtual bool Stop();
        virtual bool IsClosed() = 0;

        virtual void AddReactor(std::shared_ptr<Reactor> reactor);

    protected:
        virtual bool Close() = 0;
        virtual void ReceiveMessage();

        virtual void RemoveReactor(std::shared_ptr<Reactor> reactor);
        virtual void RemoveAllReactors();
        
    protected:
        Emittable<void> mClosed;
        std::recursive_mutex mLock;
        std::set<std::shared_ptr<Reactor>> mActiveReactors;
        TaskPtr mReceiveTask;
        std::atomic<uint32_t> mReceiveBufferSize;
    };
} }


#endif // sg_microreactor_Connection
