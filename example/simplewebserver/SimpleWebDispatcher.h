#pragma once
#ifndef simplewebserver_SimpleWebDispatcher
#define simplewebserver_SimpleWebDispatcher

#include "RestDispatcher.h"


namespace simplewebserver
{
    class SimpleWebDispatcher : public sg::microreactor::RestDispatcher
    {
    public:
        explicit SimpleWebDispatcher();
        virtual ~SimpleWebDispatcher();

    protected:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateGetFileReactor(std::shared_ptr<sg::microreactor::RestMessage> request, sg::microreactor::Connection& connection);
    };
}


#endif // simplewebserver_SimpleWebDispatcher
