#include "StreetGangRestResponseTextEncoder.h"
#include "Serializer.h"
#include "RestResponse.h"

using namespace sg::microreactor;
using namespace streetgangapi;


StreetGangRestResponseTextEncoder::StreetGangRestResponseTextEncoder()
{
}

StreetGangRestResponseTextEncoder::~StreetGangRestResponseTextEncoder()
{
}

bool StreetGangRestResponseTextEncoder::EncodeMessage(std::shared_ptr<ResponseError> message, std::ostream& stream)
{
    if (message == nullptr)
    {
        return false;
    }

    RestResponse response;
    response.mVersion = "HTTP/1.1";
    response.mStatusCode = 500;
    response.mStatusText = "Interanl Server Error";
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "text/plain"));
    response.mBody = "RequestId: " + std::to_string(message->RequestId.cref()) + "\n";
    response.mBody += "Error Message: " + message->ErrorMessage.cref() + "\n";

    return response.FlushToStream(stream);
}

bool StreetGangRestResponseTextEncoder::EncodeMessage(std::shared_ptr<ResponseGetVersion> message, std::ostream& stream)
{
    if (message == nullptr)
    {
        return false;
    }

    RestResponse response;
    response.mVersion = "HTTP/1.1";
    response.mStatusCode = 200;
    response.mStatusText = "OK";
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "text/plain"));
    response.mBody = "Version = " + message->Version.cref() + "\n";

    return response.FlushToStream(stream);
}

bool StreetGangRestResponseTextEncoder::EncodeMessage(std::shared_ptr<ResponseCreateWorld> message, std::ostream& stream)
{
    if (message == nullptr)
    {
        return false;
    }

    RestResponse response;
    response.mVersion = "HTTP/1.1";
    response.mStatusCode = 200;
    response.mStatusText = "OK";
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "text/plain"));
    response.mBody = "WorldId = " + std::to_string(message->WorldId.cref()) + "\n";
    response.mBody += "WorldName = " + message->WorldName.cref() + "\n";

    return response.FlushToStream(stream);
}

bool StreetGangRestResponseTextEncoder::EncodeMessage(std::shared_ptr<ResponseGetScene> message, std::ostream& stream)
{
    if (message == nullptr)
    {
        return false;
    }

    std::stringstream bodyStream;
    bodyStream << "WorldId = " << message->WorldId.cref() << "\n";
    bodyStream << "Rect = " << message->Rect->mX << ", " << message->Rect->mY << ", " << message->Rect->mW << ", " << message->Rect->mH << "\n";

    for (size_t i = 0; i < message->Items->size(); i++)
    {
        bodyStream << "Item[" << i << "] = " << message->Items->at(i).mX << ", " << message->Items->at(i).mY << ", " << message->Items->at(i).mZ << "\n";
    }

    RestResponse response;
    response.mVersion = "HTTP/1.1";
    response.mStatusCode = 200;
    response.mStatusText = "OK";
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "text/plain"));
    response.mBody = "WorldId = " + std::to_string(message->WorldId.cref()) + "\n";
    response.mBody += bodyStream.str();

    return response.FlushToStream(stream);
}
