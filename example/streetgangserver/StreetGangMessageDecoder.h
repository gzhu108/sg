#pragma once
#ifndef streetgangserver_StreetGangMessageDecoder
#define streetgangserver_StreetGangMessageDecoder

#include "MessageDecoder.h"
#include "BinarySerializer.h"
#include "StreetGangMessageResponseEncoder.h"
#include "RequestByebye.h"
#include "RequestGetVersion.h"
#include "RequestCreateWorld.h"
#include "RequestGetScene.h"


namespace streetgangserver
{
    class StreetGangMessageDecoder : public sg::microreactor::MessageDecoder
    {
    public:
        StreetGangMessageDecoder();
        virtual ~StreetGangMessageDecoder();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        virtual bool DecodeMessage(std::istream& stream, sg::microreactor::Message& message);
        virtual bool DecodeMessage(std::istream& stream, streetgangapi::MessageBase& message);
        virtual bool DecodeMessage(std::istream& stream, streetgangapi::RequestByebye& message);
        virtual bool DecodeMessage(std::istream& stream, streetgangapi::RequestGetVersion& message);
        virtual bool DecodeMessage(std::istream& stream, streetgangapi::RequestCreateWorld& message);
        virtual bool DecodeMessage(std::istream& stream, streetgangapi::RequestGetScene& message);

    protected:
        sg::microreactor::BinarySerializer mSerializer;
        std::shared_ptr<streetgangapi::StreetGangMessageResponseEncoder> mStreetGangMessageResponseEncoder;
    };
}

#endif // streetgangserver_StreetGangMessageDecoder
