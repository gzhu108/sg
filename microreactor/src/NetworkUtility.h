#pragma once
#ifndef sg_microreactor_NetworkUtility
#define sg_microreactor_NetworkUtility

#include <vector>
#include "Profile.h"
#include "Socket.h"
#include "Connection.h"
#include "Endpoint.h"


namespace sg { namespace microreactor
{
    namespace NetworkUtility
    {
        std::shared_ptr<Connection> CreateConnection(std::shared_ptr<Dispatcher> dispatcher);
        std::shared_ptr<Endpoint> CreateEndpoint(std::shared_ptr<Dispatcher> dispatcher);

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
}}

#endif //_MirrorLinkService_Shared_NetworkAdapterFinder_
