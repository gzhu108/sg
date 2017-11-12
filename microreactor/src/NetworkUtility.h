#pragma once
#ifndef sg_microreactor_NetworkUtility
#define sg_microreactor_NetworkUtility

#include <vector>
#include "Profile.h"
#include "Connection.h"
#include "Endpoint.h"


namespace sg { namespace microreactor
{
    namespace NetworkUtility
    {
        std::shared_ptr<Connection> CreateConnection(std::shared_ptr<Profile> profile);
        std::shared_ptr<Endpoint> CreateEndpoint(std::shared_ptr<Profile> profile);

        struct NetworkInterfaceInfo
        {
            std::string mName;
            std::string mAddress;
            uint32_t mIndex{ 0 };
            uint32_t mType{ 0 };
        };

        bool GetNetworkInterfaceInfo(std::vector<NetworkInterfaceInfo>& networkInterfaceInfoList);
    }
}}

#endif //_MirrorLinkService_Shared_NetworkAdapterFinder_
