#pragma once
#ifndef simplewebserver_SimpleWebService
#define simplewebserver_SimpleWebService

#include "RestService.h"


namespace simplewebserver
{
    class SimpleWebService : public sg::microreactor::RestService
    {
    public:
        SimpleWebService(std::shared_ptr<sg::microreactor::Host> host, std::shared_ptr<sg::microreactor::Profile> profile);
        virtual ~SimpleWebService();
    };
}


#endif // simplewebserver_SimpleWebService
