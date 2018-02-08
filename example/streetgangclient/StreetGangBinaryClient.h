#pragma once
#ifndef streetgangclient_StreetGangBinaryClient
#define streetgangclient_StreetGangBinaryClient

#include "Client.h"
#include "StreetGangRequestEncoder.h"


namespace streetgangclient
{
    class StreetGangBinaryClient : public sg::microreactor::Client
    {
    public:
        StreetGangBinaryClient(const std::string& protocol, const std::string& hostName, uint16_t port);
        virtual ~StreetGangBinaryClient();
   };
}

#endif // streetgangclient_StreetGangClient
