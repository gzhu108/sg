#pragma once
#ifndef sg_microreactor_MessageDispatcher
#define sg_microreactor_MessageDispatcher

#include "TaskManagerSingleton.h"
#include "Dispatcher.h"
#include "Reactor.h"
#include "Serializer.h"


namespace sg { namespace microreactor
{
    template <typename Key = int32_t, typename InputStream = std::istream>
    class MessageDispatcher : public Dispatcher
    {
    public:
        MessageDispatcher() {}
        virtual ~MessageDispatcher() {}

        typedef std::function<std::shared_ptr<Reactor>(InputStream&, Connection&)> Factory;

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
                if (reactor != nullptr)
                {
                    connection.AddReactor(reactor);

                    try
                    {
                        reactor->Process();
                        reactor->CheckComplete();
                    }
                    catch (...)
                    {
                        LOG("Exception: Failed to run Message Reactor's Process()");
                    }
                }

                // If no more data CAN be read (currentSize == previousSize), 
                // break out to prevent stuck in the infinite loop.
                currentSize = GetStreamSize(mReceiveStream);
            }
        }

        virtual void RegisterMessageReactorFactory(const Key& key, Factory factory)
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

        virtual Factory GetMessageReactorFactory(const Key& key)
        {
            auto found = mMessageReactorFactoryTable.find(key);
            if (found == mMessageReactorFactoryTable.end())
            {
                return nullptr;
            }

            return found->second;
        }

    protected:
        std::map<Key, Factory> mMessageReactorFactoryTable;
        std::stringstream mReceiveStream;
    };
}}


#endif // sg_microreactor_MessageDispatcher
