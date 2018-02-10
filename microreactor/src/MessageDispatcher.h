#pragma once
#ifndef sg_microreactor_MessageDispatcher
#define sg_microreactor_MessageDispatcher

#include "TaskManagerSingleton.h"
#include "Dispatcher.h"
#include "Reactor.h"
#include "Serializer.h"


namespace sg { namespace microreactor
{
    typedef std::function<std::shared_ptr<Reactor>(std::istream&, Connection&)> MessageReactorFactory;

    template <typename Key = int32_t>
    class MessageDispatcher : public Dispatcher
    {
    public:
        MessageDispatcher() {}
        virtual ~MessageDispatcher() {}

    public:
        virtual void Dispatch(Connection& connection) override
        {
            // Receive data from the connection
            if (!connection.Receive(mReceiveStream))
            {
                return;
            }

            uint64_t previousSize = 0;
            uint64_t currentSize = GetStreamSize(mReceiveStream);

            while (currentSize && currentSize != previousSize)
            {
                previousSize = currentSize;

                auto reactor = Decode(mReceiveStream, connection);
                if (reactor != nullptr && InitializeReactor(*reactor))
                {
                    // Submit shared_ptr reactor process to task queue so it's reference counted.
                    SUBMIT(std::bind(&Reactor::Process, reactor), reactor, &connection, "Reactor::Process");

                    // Call reactor process directly for better response time.
                    //reactor->Process();
                }

                // If no more data CAN be read (currentSize == previousSize), 
                // break out to prevant stucking in the infinite loop.
                currentSize = GetStreamSize(mReceiveStream);
            }
        }

        virtual void RegisterMessageReactorFactory(const Key& key, MessageReactorFactory factory)
        {
            if (factory == nullptr)
            {
                return;
            }

            auto found = mMessageReactorFactoryTable.find(key);
            if (found == mMessageReactorFactoryTable.end())
            {
                mMessageReactorFactoryTable[key] = factory;
            }
            else
            {
                LOG("Multiple factory found");

                // Overwrite the factory
                found->second = factory;
            }
        }

    protected:
        virtual std::shared_ptr<Reactor> Decode(std::istream& stream, Connection& connection) = 0;

        virtual MessageReactorFactory GetMessageReactorFactory(const Key& key)
        {
            auto found = mMessageReactorFactoryTable.find(key);
            if (found == mMessageReactorFactoryTable.end())
            {
                return nullptr;
            }

            return found->second;
        }

    protected:
        std::map<Key, MessageReactorFactory> mMessageReactorFactoryTable;
        std::stringstream mReceiveStream;
    };
}}


#endif // sg_microreactor_MessageDispatcher