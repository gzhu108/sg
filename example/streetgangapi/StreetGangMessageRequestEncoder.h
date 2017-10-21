#pragma once
#ifndef streetgangapi_StreetGangMessageRequestEncoder
#define streetgangapi_StreetGangMessageRequestEncoder

#include "StreetGangRequestEncoder.h"
#include "BinarySerializer.h"


namespace streetgangapi
{
    class StreetGangMessageRequestEncoder : public StreetGangRequestEncoder
    {
    public:
        StreetGangMessageRequestEncoder();
        virtual ~StreetGangMessageRequestEncoder();

    public:
        virtual bool EncodeMessage(std::shared_ptr<RequestByebye> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<RequestGetVersion> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<RequestCreateWorld> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<RequestGetScene> message, std::ostream& stream) override;

    protected:
        virtual bool EncodeMessage(std::shared_ptr<MessageBase> message, std::istream& inStream, std::ostream& outStream);
        virtual bool EncodeMessage(sg::microreactor::Message& message, std::ostream& stream);
        virtual bool EncodeMessage(MessageBase& message, std::ostream& stream);

    protected:
        sg::microreactor::BinarySerializer mSerializer;
    };
}

#endif // streetgangapi_StreetGangMessageRequestEncoder
