#pragma once
#ifndef streetgangapi_StreetGangMessageResponseEncoder
#define streetgangapi_StreetGangMessageResponseEncoder

#include "StreetGangResponseEncoder.h"
#include "BinarySerializer.h"


namespace streetgangapi
{
    class StreetGangMessageResponseEncoder : public StreetGangResponseEncoder
    {
    public:
        StreetGangMessageResponseEncoder();
        virtual ~StreetGangMessageResponseEncoder();

    public:
        virtual bool EncodeMessage(std::shared_ptr<ResponseError> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<ResponseGetVersion> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<ResponseCreateWorld> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<ResponseGetScene> message, std::ostream& stream) override;

    protected:
        virtual bool EncodeMessage(std::shared_ptr<MessageBase> message, std::istream& inStream, std::ostream& outStream);
        virtual bool EncodeMessage(sg::microreactor::Message& message, std::ostream& stream);
        virtual bool EncodeMessage(MessageBase& message, std::ostream& stream);

    protected:
        sg::microreactor::BinarySerializer mSerializer;
    };
}

#endif // streetgangapi_StreetGangMessageResponseEncoder
