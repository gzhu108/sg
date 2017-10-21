#pragma once
#ifndef streetgangclient_StreetGangClient
#define streetgangclient_StreetGangClient

#include "Client.h"
#include "StreetGangRequestEncoder.h"


namespace streetgangclient
{
    class StreetGangClient : public sg::microreactor::Client
    {
    public:
        StreetGangClient(const std::string& protocol, const std::string& hostName, uint16_t port);
        virtual ~StreetGangClient();

        std::shared_ptr<streetgangapi::StreetGangRequestEncoder> GetStreetGangRequestEncoder() { return mStreetGangRequestEncoder; }

    protected:
        std::shared_ptr<streetgangapi::StreetGangRequestEncoder> mStreetGangRequestEncoder;
   };
}

#endif // streetgangclient_StreetGangClient
