#pragma once
#ifndef streetgangapi_StreetGangRestResponseTextEncoder
#define streetgangapi_StreetGangRestResponseTextEncoder

#include "StreetGangResponseEncoder.h"


namespace streetgangapi
{
    class StreetGangRestResponseTextEncoder : public StreetGangResponseEncoder
    {
    public:
        StreetGangRestResponseTextEncoder();
        virtual ~StreetGangRestResponseTextEncoder();

    public:
        virtual bool EncodeMessage(std::shared_ptr<ResponseError> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<ResponseGetVersion> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<ResponseCreateWorld> message, std::ostream& stream) override;
        virtual bool EncodeMessage(std::shared_ptr<ResponseGetScene> message, std::ostream& stream) override;
    };
}

#endif // streetgangapi_StreetGangRestResponseTextEncoder
