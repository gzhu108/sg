#pragma once
#ifndef streetgangserver_RequestCreateWorldReactor
#define streetgangserver_RequestCreateWorldReactor

#include "MessageReactor.h"
#include "StreetGangResponseEncoder.h"
#include "RequestCreateWorld.h"


namespace streetgangserver
{
    class RequestCreateWorldReactor : public sg::microreactor::MessageReactor<streetgangapi::RequestCreateWorld, streetgangapi::StreetGangResponseEncoder>
    {
    public:
        RequestCreateWorldReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::RequestCreateWorld> message);
        virtual ~RequestCreateWorldReactor();
        
    public:
        virtual bool Process() override;
    };
}


#endif // streetgangserver_RequestCreateWorldReactor
