#pragma once
#ifndef streetgangclient_StreetGangPBClient
#define streetgangclient_StreetGangPBClient

#include "Client.h"


namespace streetgangclient
{
    class StreetGangPBClient : public sg::microreactor::Client
    {
    public:
        StreetGangPBClient(const std::string& protocol, const std::string& hostAddress, uint16_t port);
        virtual ~StreetGangPBClient();
   };
}

#endif // streetgangclient_StreetGangClient
