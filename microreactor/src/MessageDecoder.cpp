#include "MessageDecoder.h"
#include "Connection.h"
#include "Reactor.h"
#include "Serializer.h"
#include "TaskManagerSingleton.h"

using namespace sg::microreactor;


MessageDecoder::MessageDecoder()
{
}

MessageDecoder::~MessageDecoder()
{
}

void MessageDecoder::Dispatch(Connection& connection)
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
