#pragma once
#ifndef worldserver_CreateWorldReactor
#define worldserver_CreateWorldReactor

#include "MessageReactor.h"
#include "WorldResponder.h"
#include "RequestCreateWorld.h"


namespace worldserver
{
    class CreateWorldReactor : public sg::microreactor::MessageReactor<worldapi::RequestCreateWorld>
    {
    public:
        CreateWorldReactor(std::shared_ptr<sg::microreactor::Connection> connection, std::shared_ptr<worldapi::RequestCreateWorld> message, std::shared_ptr<worldapi::WorldResponder> responder);
        virtual ~CreateWorldReactor();
        
    public:
        virtual bool Process() override;

    protected:
        std::shared_ptr<worldapi::WorldResponder> mResponder;
    };
}


#endif // worldserver_CreateWorldReactor
