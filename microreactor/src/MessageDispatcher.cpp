#include "MessageDispatcher.h"
#include "TaskManagerSingleton.h"
#include "Serializer.h"

using namespace microreactor;


MessageDispatcher::MessageDispatcher()
{
}

MessageDispatcher::~MessageDispatcher()
{
}

void MessageDispatcher::Dispatch(Connection& connection)
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
