#pragma once
#ifndef streetgangapi_StreetGangRestResponseJsonEncoder
#define streetgangapi_StreetGangRestResponseJsonEncoder

#include "ResponseError.h"
#include "ResponseGetVersion.h"
#include "ResponseCreateWorld.h"
#include "ResponseGetScene.h"


namespace streetgangapi
{
    class StreetGangRestResponseJsonEncoder
    {
    public:
        StreetGangRestResponseJsonEncoder();
        virtual ~StreetGangRestResponseJsonEncoder();

    public:
        virtual bool EncodeMessage(std::shared_ptr<ResponseError> message, std::ostream& stream);
        virtual bool EncodeMessage(std::shared_ptr<ResponseGetVersion> message, std::ostream& stream);
        virtual bool EncodeMessage(std::shared_ptr<ResponseCreateWorld> message, std::ostream& stream);
        virtual bool EncodeMessage(std::shared_ptr<ResponseGetScene> message, std::ostream& stream);
    };
}

#endif // streetgangapi_StreetGangRestResponseJsonEncoder
