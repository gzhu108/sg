#pragma once
#ifndef streetgangapi_StreetGangRestResponseJsonEncoder
#define streetgangapi_StreetGangRestResponseJsonEncoder

#include "StreetGangResponseEncoder.h"


namespace streetgangapi
{
    class StreetGangRestResponseJsonEncoder : public StreetGangResponseEncoder
    {
    public:
        StreetGangRestResponseJsonEncoder();
        virtual ~StreetGangRestResponseJsonEncoder();

    public:
        virtual bool EncodeMessage(std::shared_ptr<ResponseError> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<ResponseGetVersion> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<ResponseCreateWorld> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<ResponseGetScene> message, std::ostream& stream) override;
    };
}

#endif // streetgangapi_StreetGangRestResponseJsonEncoder
