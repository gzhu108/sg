#pragma once
#ifndef worldserver_GetWorldReactor
#define worldserver_GetWorldReactor

#include "MessageReactor.h"
#include "WorldResponder.h"
#include "RequestGetWorld.h"
#include "WorldProvider.h"


namespace worldserver
{
    class GetWorldReactor : public sg::microreactor::MessageReactor<worldapi::RequestGetWorld>
    {
    public:
        GetWorldReactor(sg::microreactor::Connection& connection, std::shared_ptr<worldapi::RequestGetWorld> message, std::shared_ptr<worldapi::WorldResponder> responder);
        virtual ~GetWorldReactor();
        
    public:
        virtual bool Process() override;

    protected:
        std::shared_ptr<worldapi::WorldResponder> mResponder;
    };
}


#endif // worldserver_GetWorldReactor
