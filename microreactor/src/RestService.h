#pragma once
#ifndef sg_microreactor_RestService
#define sg_microreactor_RestService

#include "Service.h"
#include "RestDispatcher.h"


namespace sg { namespace microreactor
{
    class RestService : public Service
    {
    public:
        RestService(const std::string& hostAddress, uint16_t port);
        explicit RestService(std::shared_ptr<Profile> profile);
        explicit RestService(std::shared_ptr<Listener> listener);
        virtual ~RestService();
    };
}}


#endif // sg_microreactor_RestService
