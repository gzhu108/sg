#pragma once
#ifndef microreactor_NetworkUtility
#define microreactor_NetworkUtility

#include <vector>
#include "Socket.h"
#include "Dispatcher.h"
#include "Connection.h"
#include "Listener.h"


namespace microreactor
{
    namespace NetworkUtility
    {
        std::shared_ptr<Connection> CreateConnection(std::shared_ptr<Dispatcher> dispatcher);
        std::shared_ptr<Listener> CreateListener(std::shared_ptr<Dispatcher> dispatcher);

        struct NetworkInterfaceInfo
        {
            std::string mName;
            std::string mAddress;
            uint32_t mIndex{ 0 };
            uint32_t mType{ 0 };
            uint16_t mFamily{ 0 };
        };

        bool GetNetworkInterfaceInfo(std::vector<NetworkInterfaceInfo>& networkInterfaceInfoList);
        std::shared_ptr<addrinfo> GetAddressInfo(const std::string& address, uint16_t port, int32_t type, int32_t protocol, bool forBinding);
    }
}

#endif //_MirrorLinkService_Shared_NetworkAdapterFinder_
