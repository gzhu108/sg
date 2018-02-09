#pragma once
#ifndef streetgangapi_StreetGangRestResponseTextEncoder
#define streetgangapi_StreetGangRestResponseTextEncoder

#include "ResponseError.h"
#include "ResponseGetVersion.h"
#include "ResponseCreateWorld.h"
#include "ResponseGetScene.h"


namespace streetgangapi
{
    class StreetGangRestResponseTextEncoder
    {
    public:
        StreetGangRestResponseTextEncoder();
        virtual ~StreetGangRestResponseTextEncoder();

    public:
        virtual bool EncodeMessage(std::shared_ptr<ResponseError> message, std::ostream& stream);
        virtual bool EncodeMessage(std::shared_ptr<ResponseGetVersion> message, std::ostream& stream);
        virtual bool EncodeMessage(std::shared_ptr<ResponseCreateWorld> message, std::ostream& stream);
        virtual bool EncodeMessage(std::shared_ptr<ResponseGetScene> message, std::ostream& stream);
    };
}

#endif // streetgangapi_StreetGangRestResponseTextEncoder
