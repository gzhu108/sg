#pragma once
#ifndef streetgangserver_StreetGangPBDecoder
#define streetgangserver_StreetGangPBDecoder

#include "MessageDecoder.h"
#include "StreetGangPBResponseEncoder.h"
#include "RequestByebye.h"
#include "RequestGetVersion.h"
#include "RequestCreateWorld.h"
#include "RequestGetScene.h"


namespace streetgangserver
{
    class StreetGangPBDecoder : public sg::microreactor::MessageDecoder
    {
    public:
        StreetGangPBDecoder();
        virtual ~StreetGangPBDecoder();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<streetgangapi::StreetGangPBResponseEncoder> mStreetGangPBResponseEncoder;
    };
}

#endif // streetgangserver_StreetGangPBDecoder
