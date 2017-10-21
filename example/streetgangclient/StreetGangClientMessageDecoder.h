#pragma once
#ifndef streetgangclient_StreetGangClientMessageDecoder
#define streetgangclient_StreetGangClientMessageDecoder

#include "MessageDecoder.h"
#include "BinarySerializer.h"
#include "StreetGangMessageRequestEncoder.h"

#include "ResponseError.h"
#include "ResponseGetVersion.h"
#include "ResponseCreateWorld.h"
#include "ResponseGetScene.h"


namespace streetgangclient
{
    class StreetGangClientMessageDecoder : public sg::microreactor::MessageDecoder
    {
    public:
        StreetGangClientMessageDecoder();
        virtual ~StreetGangClientMessageDecoder();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        virtual bool DecodeMessage(std::istream& stream, sg::microreactor::Message& message);
        virtual bool DecodeMessage(std::istream& stream, streetgangapi::MessageBase& message);
        virtual bool DecodeMessage(std::istream& stream, streetgangapi::ResponseError& message);
        virtual bool DecodeMessage(std::istream& stream, streetgangapi::ResponseGetVersion& message);
        virtual bool DecodeMessage(std::istream& stream, streetgangapi::ResponseCreateWorld& message);
        virtual bool DecodeMessage(std::istream& stream, streetgangapi::ResponseGetScene& message);

    protected:
        sg::microreactor::BinarySerializer mSerializer;
        std::shared_ptr<streetgangapi::StreetGangMessageRequestEncoder> mStreetGangMessageRequestEncoder;
    };
}

#endif // streetgangclient_StreetGangClientReactorFactory
