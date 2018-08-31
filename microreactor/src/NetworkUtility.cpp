#include "NetworkUtility.h"
#include "TcpConnection.h"
#include "UdpConnection.h"
#include "TcpEndpoint.h"
#include "UdpEndpoint.h"

using namespace sg::microreactor;


std::shared_ptr<Connection> NetworkUtility::CreateConnection(std::shared_ptr<Profile> profile)
{
    if (profile == nullptr)
    {
        return nullptr;
    }

    if (profile->Protocol.cref() == "tcp")
    {
        return std::make_shared<TcpConnection>(nullptr, profile);
    }
    else if (profile->Protocol.cref() == "udp")
    {
        return std::make_shared<UdpConnection>(nullptr, profile);
    }
    else
    {
        return nullptr;
    }
}


std::shared_ptr<Endpoint> NetworkUtility::CreateEndpoint(std::shared_ptr<Profile> profile)
{
    if (profile == nullptr)
    {
        return nullptr;
    }

    if (profile->Protocol.cref() == "tcp")
    {
        auto endpoint = std::make_shared<TcpEndpoint>(nullptr, profile);
        LOG("TCP HOST: %s", endpoint->Name->c_str());
        return endpoint;
    }
    else if (profile->Protocol.cref() == "udp")
    {
        auto endpoint = std::make_shared<UdpEndpoint>(nullptr, profile);
        LOG("UDP HOST: %s", endpoint->Name->c_str());
        return endpoint;
    }
    else
    {
        return nullptr;
    }
}


#ifdef _MSC_VER   // Windows implementation
#include <WinSock2.h>
#include <windows.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

/*

#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

// Link with Iphlpapi.lib
#pragma comment(lib, "IPHLPAPI.lib")

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

// Note: could also use malloc() and free()

int __cdecl main(int argc, char **argv)
{

    // Declare and initialize variables 

    DWORD dwSize = 0;
    DWORD dwRetVal = 0;

    unsigned int i = 0;

    // Set the flags to pass to GetAdaptersAddresses
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

    // default to unspecified address family (both)
    ULONG family = AF_UNSPEC;

    LPVOID lpMsgBuf = NULL;

    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    ULONG outBufLen = 0;
    ULONG Iterations = 0;

    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
    IP_ADAPTER_PREFIX *pPrefix = NULL;

    if (argc != 2) {
        LOG(" Usage: getadapteraddresses family");
        LOG("        getadapteraddresses 4 (for IPv4)");
        LOG("        getadapteraddresses 6 (for IPv6)");
        LOG("        getadapteraddresses A (for both IPv4 and IPv6)");
        exit(1);
    }

    if (atoi(argv[1]) == 4)
        family = AF_INET;
    else if (atoi(argv[1]) == 6)
        family = AF_INET6;

    LOG("Calling GetAdaptersAddresses function with family = ");
    if (family == AF_INET)
        LOG("AF_INET");
    if (family == AF_INET6)
        LOG("AF_INET6");
    if (family == AF_UNSPEC)
        LOG("AF_UNSPEC\n");

    // Allocate a 15 KB buffer to start with.
    outBufLen = WORKING_BUFFER_SIZE;

    do {

        pAddresses = (IP_ADAPTER_ADDRESSES *)MALLOC(outBufLen);
        if (pAddresses == NULL) {
            LOG
                ("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
            exit(1);
        }

        dwRetVal =
            GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

        if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
            FREE(pAddresses);
            pAddresses = NULL;
        }
        else {
            break;
        }

        Iterations++;

    } while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

    if (dwRetVal == NO_ERROR) {
        // If successful, output some information from the data we received
        pCurrAddresses = pAddresses;
        while (pCurrAddresses) {
            LOG("\tLength of the IP_ADAPTER_ADDRESS struct: %ld",
                pCurrAddresses->Length);
            LOG("\tIfIndex (IPv4 interface): %u", pCurrAddresses->IfIndex);
            LOG("\tAdapter name: %s", pCurrAddresses->AdapterName);

            pUnicast = pCurrAddresses->FirstUnicastAddress;
            if (pUnicast != NULL) {
                for (i = 0; pUnicast != NULL; i++)
                    pUnicast = pUnicast->Next;
                LOG("\tNumber of Unicast Addresses: %d", i);
            }
            else
                LOG("\tNo Unicast Addresses");

            pAnycast = pCurrAddresses->FirstAnycastAddress;
            if (pAnycast) {
                for (i = 0; pAnycast != NULL; i++)
                    pAnycast = pAnycast->Next;
                LOG("\tNumber of Anycast Addresses: %d", i);
            }
            else
                LOG("\tNo Anycast Addresses");

            pMulticast = pCurrAddresses->FirstMulticastAddress;
            if (pMulticast) {
                for (i = 0; pMulticast != NULL; i++)
                    pMulticast = pMulticast->Next;
                LOG("\tNumber of Multicast Addresses: %d", i);
            }
            else
                LOG("\tNo Multicast Addresses");

            pDnServer = pCurrAddresses->FirstDnsServerAddress;
            if (pDnServer) {
                for (i = 0; pDnServer != NULL; i++)
                    pDnServer = pDnServer->Next;
                LOG("\tNumber of DNS Server Addresses: %d", i);
            }
            else
                LOG("\tNo DNS Server Addresses");

            LOG("\tDNS Suffix: %wS", pCurrAddresses->DnsSuffix);
            LOG("\tDescription: %wS", pCurrAddresses->Description);
            LOG("\tFriendly name: %wS", pCurrAddresses->FriendlyName);

            if (pCurrAddresses->PhysicalAddressLength != 0) {
                LOG("\tPhysical address: ");
                for (i = 0; i < (int)pCurrAddresses->PhysicalAddressLength;
                i++) {
                    if (i == (pCurrAddresses->PhysicalAddressLength - 1))
                        LOG("%.2X",
                            (int)pCurrAddresses->PhysicalAddress[i]);
                    else
                        LOG("%.2X-",
                            (int)pCurrAddresses->PhysicalAddress[i]);
                }
            }
            LOG("\tFlags: %ld", pCurrAddresses->Flags);
            LOG("\tMtu: %lu", pCurrAddresses->Mtu);
            LOG("\tIfType: %ld", pCurrAddresses->IfType);
            LOG("\tOperStatus: %ld", pCurrAddresses->OperStatus);
            LOG("\tIpv6IfIndex (IPv6 interface): %u",
                pCurrAddresses->Ipv6IfIndex);
            LOG("\tZoneIndices (hex): ");
            for (i = 0; i < 16; i++)
                LOG("%lx ", pCurrAddresses->ZoneIndices[i]);
            LOG("");

            LOG("\tTransmit link speed: %I64u", pCurrAddresses->TransmitLinkSpeed);
            LOG("\tReceive link speed: %I64u", pCurrAddresses->ReceiveLinkSpeed);

            pPrefix = pCurrAddresses->FirstPrefix;
            if (pPrefix) {
                for (i = 0; pPrefix != NULL; i++)
                    pPrefix = pPrefix->Next;
                LOG("\tNumber of IP Adapter Prefix entries: %d", i);
            }
            else
                LOG("\tNumber of IP Adapter Prefix entries: 0");

            LOG("");

            pCurrAddresses = pCurrAddresses->Next;
        }
    }
    else {
        LOG("Call to GetAdaptersAddresses failed with error: %d",
            dwRetVal);
        if (dwRetVal == ERROR_NO_DATA)
            LOG("\tNo addresses were found for the requested parameters");
        else {

            if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                // Default language
                (LPTSTR)& lpMsgBuf, 0, NULL)) {
                LOG("\tError: %s", lpMsgBuf);
                LocalFree(lpMsgBuf);
                if (pAddresses)
                    FREE(pAddresses);
                exit(1);
            }
        }
    }

    if (pAddresses) {
        FREE(pAddresses);
    }

    return 0;
}

*/

bool NetworkUtility::GetNetworkInterfaceInfo(std::vector<NetworkInterfaceInfo>& networkInterfaceInfoList)
{
    /* Declare and initialize variables */

    // It is possible for an adapter to have multiple
    // IPv4 addresses, gateways, and secondary WINS servers
    // assigned to the adapter.
    //
    // Note that this sample code only prints out the
    // first entry for the IP address/mask, and gateway, and
    // the primary and secondary WINS server for each adapter.

    bool result = false;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO*)HeapAlloc(GetProcessHeap(), 0, ulOutBufLen);

    // Make an initial call to GetAdaptersInfo to get
    // the necessary size into the ulOutBufLen variable
    DWORD dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
    if (dwRetVal == ERROR_BUFFER_OVERFLOW)
    {
        HeapFree(GetProcessHeap(), 0, pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*)HeapAlloc(GetProcessHeap(), 0, ulOutBufLen);
        dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
    }

    if (dwRetVal == NO_ERROR)
    {
        PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
        while (pAdapter)
        {
            NetworkInterfaceInfo interfaceInfo;

            // Example: quick detection, specific for my laptop
            //if (0 == strcmp(pAdapter->Description, "VMware Virtual Ethernet Adapter for VMnet8"))
            //if (0 == strcmp(pAdapter->Description, "Ethernet adapter Local Area Connection"))
            //if (0 == strcmp(pAdapter->Description, "Intel(R) 82577LM Gigabit Network Connection"))
            //if (0 == strcmp(pAdapter->Description, "Intel(R) Centrino(R) Ultimate-N 6300 AGN"))
            //if (0 == strcmp(pAdapter->Description, "Belkin 54g Wireless USB Network Adapter #2"))
            //if (0 == strcmp(pAdapter->Description, "Belkin 54g Wireless USB Network Adapter"))

            interfaceInfo.mType = pAdapter->Type;
            interfaceInfo.mName = pAdapter->Description;
            interfaceInfo.mAddress = pAdapter->IpAddressList.IpAddress.String;
            interfaceInfo.mIndex = pAdapter->Index;
            networkInterfaceInfoList.emplace_back(interfaceInfo);

            pAdapter = pAdapter->Next;
        }

        result = true;
    }

    HeapFree(GetProcessHeap(), 0, pAdapterInfo);
    return result;
}

#else   // Linux implementation

#include <sys/types.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

bool NetworkUtility::GetNetworkInterfaceInfo(std::vector<NetworkInterfaceInfo>& networkInterfaceInfoList)
{
    bool result = true;
    ifaddrs* ifaddr = nullptr;
    ifaddrs* ifa = nullptr;
    int family = 0;
    int s = 0;
    char host[NI_MAXHOST] = { 0 };

    if (getifaddrs(&ifaddr) == -1)
    {
        return false;
    }

    /* Walk through linked list, maintaining head pointer so we
       can free list later */

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
        {
            continue;
        }

        family = ifa->ifa_addr->sa_family;

        /* Display interface name and family (including symbolic
            form of the latter for the common families) */

        LOG("%s  address family: %d%s",
            ifa->ifa_name, family,
            (family == AF_PACKET) ? " (AF_PACKET)" :
            (family == AF_INET) ?   " (AF_INET)" :
            (family == AF_INET6) ?  " (AF_INET6)" : "");

        /* For an AF_INET* interface address, display the address */

        if (family == AF_INET || family == AF_INET6)
        {
            s = getnameinfo(ifa->ifa_addr,
                    (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
                    host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

            if (s != 0)
            {
                LOG("getnameinfo() failed: %s", gai_strerror(s));
                result = false;
                break;
            }

            LOG("\taddress: <%s>", host);
            //networkInterfaceInfoList.emplace_back();
        }
    }

    freeifaddrs(ifaddr);
    return result;
}

#endif

std::shared_ptr<addrinfo> NetworkUtility::GetAddressInfo(const std::string& address, uint16_t port, int32_t type, int32_t protocol, bool forBinding)
{
#ifdef _MSC_VER
    // Initialize Winsock
    WSADATA socketInfo = { 0 };
    WSAStartup(MAKEWORD(2, 2), &socketInfo);
#endif

    addrinfo addrHints;
    memset(&addrHints, 0, sizeof(addrHints));
    addrHints.ai_family = AF_UNSPEC;
    addrHints.ai_socktype = type;
    addrHints.ai_protocol = protocol;
    addrHints.ai_flags = AI_CANONNAME | (forBinding ? AI_PASSIVE : 0);

    const char* node = nullptr;
    if (!address.empty())
    {
        node = address.c_str();
    }

    // Resolve address info
    addrinfo* addrInfo = nullptr;
    int32_t result = getaddrinfo(node, std::to_string(port).c_str(), &addrHints, &addrInfo);
    if (result != 0)
    {
        int32_t error = GetSocketError();
        return nullptr;
    }

#ifdef _MSC_VER
    WSACleanup();
#endif

    std::shared_ptr<addrinfo> addrInfoPtr(addrInfo, freeaddrinfo);
    return addrInfoPtr;
}
