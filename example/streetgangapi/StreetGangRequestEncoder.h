#pragma once
#ifndef streetgangapi_StreetGangRequestEncoder
#define streetgangapi_StreetGangRequestEncoder

#include "RequestByebye.h"
#include "RequestGetVersion.h"
#include "RequestCreateWorld.h"
#include "RequestGetScene.h"


namespace streetgangapi
{
    class StreetGangRequestEncoder
    {
    public:
        virtual bool EncodeMessage(std::shared_ptr<RequestByebye> message, std::ostream& stream) = 0;
        virtual bool EncodeMessage(std::shared_ptr<RequestGetVersion> message, std::ostream& stream) = 0;
        virtual bool EncodeMessage(std::shared_ptr<RequestCreateWorld> message, std::ostream& stream) = 0;
        virtual bool EncodeMessage(std::shared_ptr<RequestGetScene> message, std::ostream& stream) = 0;
    };
}


#endif // streetgangapi_StreetGangRequestEncoder
