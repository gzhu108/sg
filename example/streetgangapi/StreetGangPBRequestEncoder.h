#pragma once
#ifndef streetgangapi_StreetGangPBRequestEncoder
#define streetgangapi_StreetGangPBRequestEncoder

#include "StreetGangRequestEncoder.h"


namespace streetgangapi
{
    class StreetGangPBRequestEncoder : public StreetGangRequestEncoder
    {
    public:
        StreetGangPBRequestEncoder();
        virtual ~StreetGangPBRequestEncoder();

    public:
        virtual bool EncodeMessage(std::shared_ptr<RequestByebye> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<RequestGetVersion> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<RequestCreateWorld> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<RequestGetScene> message, std::ostream& stream) override;
    };
}

#endif // streetgangapi_StreetGangPBRequestEncoder
