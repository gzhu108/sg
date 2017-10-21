#pragma once
#ifndef sg_microreactor_Connection
#define sg_microreactor_Connection

#include <chrono>
#include <set>
#include <mutex>
#include "Profile.h"
#include "Message.h"


namespace sg { namespace microreactor
{
    class Connection
    {
    public:
        explicit Connection(std::shared_ptr<Profile> profile);
        virtual ~Connection();
        
        Signal<void>& Closed = mClosed;

        PROPERTY(Name, std::string);
        PROPERTY(ReceiveTimeout, std::chrono::milliseconds, std::chrono::milliseconds(1));
        PROPERTY(SendTimeout, std::chrono::milliseconds, std::chrono::milliseconds(100));
        
    public:
        virtual std::shared_ptr<Profile> GetProfile() { return mProfile; }
        virtual void RegisterMessage(std::shared_ptr<Message> message);

        virtual std::string GetPeerName() const = 0;
        virtual uint16_t GetPeerPort() const = 0;

        virtual bool Receive(std::iostream& stream);
        virtual bool Send(std::istream& stream);

        virtual bool DataReady() = 0;
        virtual uint64_t Receive(char* buffer, int32_t length) = 0;
        virtual uint64_t Send(const char* buffer, int32_t length) = 0;
        
        virtual bool Start();
        virtual bool Stop();
        virtual bool IsClosed() = 0;

    protected:
        virtual bool Close() = 0;
        virtual void ReceiveMessage();
        virtual void CancelAllTasks(const std::chrono::microseconds& waitTime);
        
    protected:
        Emittable<void> mClosed;
        std::recursive_mutex mLock;
        std::shared_ptr<Profile> mProfile;
        std::vector<char> mReceiveBuffer;
        std::vector<char> mSendBuffer;
    };
} }


#endif // sg_microreactor_Connection
