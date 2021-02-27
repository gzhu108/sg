#pragma once
#ifndef simplewebserver_SimpleWebDispatcher
#define simplewebserver_SimpleWebDispatcher

#include "RestDispatcher.h"


namespace simplewebserver
{
    class SimpleWebDispatcher : public microreactor::RestDispatcher
    {
    public:
        explicit SimpleWebDispatcher();
        virtual ~SimpleWebDispatcher();

    protected:
        virtual std::shared_ptr<microreactor::Reactor> CreateGetFileReactor(std::shared_ptr<microreactor::RestMessage> request, microreactor::Connection& connection);
    };
}


#endif // simplewebserver_SimpleWebDispatcher
