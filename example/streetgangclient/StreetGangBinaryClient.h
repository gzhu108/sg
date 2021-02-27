#pragma once
#ifndef streetgangclient_StreetGangBinaryClient
#define streetgangclient_StreetGangBinaryClient

#include "Client.h"


namespace streetgangclient
{
    class StreetGangBinaryClient : public microreactor::Client
    {
    public:
        StreetGangBinaryClient(const std::string& protocol, const std::string& hostAddress, uint16_t port);
        virtual ~StreetGangBinaryClient();
   };
}

#endif // streetgangclient_StreetGangClient
