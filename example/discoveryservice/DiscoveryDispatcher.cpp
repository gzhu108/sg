#include "DiscoveryDispatcher.h"
#include "StringSerializer.h"
#include "StringUtility.h"
#include "RequestSearchReactor.h"

using namespace sg::microreactor;
using namespace sg::service;


DiscoveryDispatcher::DiscoveryDispatcher()
{
}

DiscoveryDispatcher::~DiscoveryDispatcher()
{
}

void DiscoveryDispatcher::Dispatch(Connection& connection)
{
    // Receive data from the connection
    if (!connection.Receive(mReceiveStream))
    {
        return;
    }

    StringSerializer serializer;
    std::string requestString;

    if (!serializer.Read(mReceiveStream, requestString))
    {
        return;
    }

    std::vector<std::string> requestLines;
    if (StringUtility::Split(requestString, std::string("\r\n"), requestLines))
    {
        requestLines[0] = StringUtility::Trim(requestLines[0], std::string(" "));

        if ("M-SEARCH * HTTP/1.1" == requestLines[0])
        {
            // put back the request string
            std::stringstream outStream;
            serializer.Write(requestString, outStream);

            auto message = std::make_shared<RequestSearch>();
            message->Deserialize(outStream, serializer);

            auto reactor = std::make_shared<RequestSearchReactor>(std::static_pointer_cast<Connection>(connection.shared_from_this()), message);
            if (reactor)
            {
                reactor->Process();
            }
        }
    }
}
