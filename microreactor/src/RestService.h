#pragma once
#ifndef microreactor_RestService
#define microreactor_RestService

#include "Service.h"
#include "RestDispatcher.h"


namespace microreactor
{
    class RestService : public Service
    {
    public:
        RestService(const std::string& hostAddress, uint16_t port);
        explicit RestService(std::shared_ptr<Profile> profile);
        explicit RestService(std::shared_ptr<Listener> listener);
        virtual ~RestService();
    };
}


#endif // microreactor_RestService
