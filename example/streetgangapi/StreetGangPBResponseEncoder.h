#pragma once
#ifndef streetgangapi_StreetGangPBResponseEncoder
#define streetgangapi_StreetGangPBResponseEncoder

#include "StreetGangResponseEncoder.h"


namespace streetgangapi
{
    class StreetGangPBResponseEncoder : public StreetGangResponseEncoder
    {
    public:
        StreetGangPBResponseEncoder();
        virtual ~StreetGangPBResponseEncoder();

    public:
        virtual bool EncodeMessage(std::shared_ptr<ResponseError> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<ResponseGetVersion> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<ResponseCreateWorld> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<ResponseGetScene> message, std::ostream& stream) override;
    };
}

#endif // streetgangapi_StreetGangPBResponseEncoder
