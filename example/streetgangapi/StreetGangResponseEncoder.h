#pragma once
#ifndef streetgangapi_StreetGangResponseEncoder
#define streetgangapi_StreetGangResponseEncoder

#include "ResponseError.h"
#include "ResponseGetVersion.h"
#include "ResponseCreateWorld.h"
#include "ResponseGetScene.h"


namespace streetgangapi
{
    class StreetGangResponseEncoder
    {
    public:
        virtual bool EncodeMessage(std::shared_ptr<ResponseError> message, std::ostream& stream) = 0;
        virtual bool EncodeMessage(std::shared_ptr<ResponseGetVersion> message, std::ostream& stream) = 0;
        virtual bool EncodeMessage(std::shared_ptr<ResponseCreateWorld> message, std::ostream& stream) = 0;
        virtual bool EncodeMessage(std::shared_ptr<ResponseGetScene> message, std::ostream& stream) = 0;
    };
}


#endif // streetgangapi_StreetGangResponseEncoder
