#pragma once
#ifndef streetgangclient_StreetGangPBClient
#define streetgangclient_StreetGangPBClient

#include "Client.h"
#include "StreetGangRequestEncoder.h"


namespace streetgangclient
{
    class StreetGangPBClient : public sg::microreactor::Client
    {
    public:
        StreetGangPBClient(const std::string& protocol, const std::string& hostName, uint16_t port);
        virtual ~StreetGangPBClient();

        std::shared_ptr<streetgangapi::StreetGangRequestEncoder> GetStreetGangRequestEncoder() { return mStreetGangRequestEncoder; }

    protected:
        std::shared_ptr<streetgangapi::StreetGangRequestEncoder> mStreetGangRequestEncoder;
   };
}

#endif // streetgangclient_StreetGangClient
