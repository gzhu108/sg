#pragma once
#ifndef streetgangserver_StreetGangPBDispatcher
#define streetgangserver_StreetGangPBDispatcher

#include "MessageDecoder.h"
#include "StreetGangPBResponseEncoder.h"
#include "RequestByebye.h"
#include "RequestGetVersion.h"
#include "RequestCreateWorld.h"
#include "RequestGetScene.h"


namespace streetgangserver
{
    class StreetGangPBDispatcher : public sg::microreactor::MessageDecoder
    {
    public:
        StreetGangPBDispatcher();
        virtual ~StreetGangPBDispatcher();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<streetgangapi::StreetGangPBResponseEncoder> mStreetGangPBResponseEncoder;
    };
}

#endif // streetgangserver_StreetGangPBDispatcher
