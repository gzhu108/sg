#include "UdpSocket.h"
#include "Exception.h"

using namespace sg::microreactor;


UdpSocket::UdpSocket()
{
}

UdpSocket::UdpSocket(SOCKET socket)
{
    if (Attach(socket))
    {
        GetSocketAddress();
        GetPeerAddress();
    }
}

UdpSocket::~UdpSocket()
{
}

bool UdpSocket::JoinMulticastGoup(const std::string& multicastAddress, const std::string& interfaceAddress, bool loopback, uint32_t timeToLive)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (!IsValid() || mAddrInfo == nullptr || multicastAddress.length() == 0)
    {
        return false;
    }

    std::shared_ptr<addrinfo> interfaceAddressInfo = nullptr;
    if (!interfaceAddress.empty())
    {
        interfaceAddressInfo = NetworkUtility::GetAddressInfo(interfaceAddress, 0, SOCK_DGRAM, IPPROTO_UDP, true);
    }

    in_addr multicastInterface = { 0 };
    in6_addr multicastInterfaceIpv6 = { 0 };

    if (interfaceAddressInfo != nullptr)
    {
        if (interfaceAddressInfo->ai_addr->sa_family == AF_INET)
        {
            inet_pton(AF_INET, interfaceAddress.c_str(), &multicastInterface);
        }
        else
        {
            inet_pton(AF_INET6, interfaceAddress.c_str(), &multicastInterfaceIpv6);
        }
    }

    uint32_t anyMulticastInterfaceIndex = INADDR_ANY;

    // Set socket multicast options
    int32_t optLevel = 0;
    int32_t option = 0;
    char* optVal = NULL;
    int32_t optLen = 0;

#ifdef _MSC_VER
    // Bind to adapter index to work around weak host model of TCPIP stack
    DWORD dw = 0;
    WSAIoctl(mSocket, SIO_INDEX_BIND, &anyMulticastInterfaceIndex, sizeof(anyMulticastInterfaceIndex), NULL, 0, &dw, NULL, NULL);
#endif

    // Join the multicast group
    if (mAddrInfo->ai_addr->sa_family == AF_INET)
    {
        // Setup the v4 option values and ip_mreq structure
        ip_mreq mreqv4;
        inet_pton(AF_INET, multicastAddress.c_str(), &mreqv4.imr_multiaddr);
        memcpy(&mreqv4.imr_interface, &multicastInterface, sizeof(multicastInterface));

        optLevel = IPPROTO_IP;
        option   = IP_ADD_MEMBERSHIP;
        optVal   = (char*)&mreqv4;
        optLen   = sizeof(mreqv4);
    }
    else if (mAddrInfo->ai_addr->sa_family == AF_INET6)
    {
        // Setup the v6 option values and ipv6_mreq structure
        ipv6_mreq mreqv6;
        inet_pton(AF_INET6, multicastAddress.c_str(), &mreqv6.ipv6mr_multiaddr);
        if (interfaceAddress.empty())
        {
            mreqv6.ipv6mr_interface = anyMulticastInterfaceIndex;
        }
        else
        {
            std::shared_ptr<addrinfo> interfaceAddressInfo = NetworkUtility::GetAddressInfo(interfaceAddress, 0, SOCK_DGRAM, IPPROTO_UDP, true);
            mreqv6.ipv6mr_interface = ((sockaddr_in6*)interfaceAddressInfo->ai_addr)->sin6_scope_id;
        }

// Define IPV6_ADD_MEMBERSHIP for FreeBSD and Mac OS X
#ifndef IPV6_ADD_MEMBERSHIP
#define IPV6_ADD_MEMBERSHIP IPV6_JOIN_GROUP
#endif

        optLevel = IPPROTO_IPV6;
        option   = IPV6_ADD_MEMBERSHIP;
        optVal   = (char*)&mreqv6;
        optLen   = sizeof(mreqv6);
    }
    
    if (SetSockOpt(optLevel, option, optVal, optLen) == SOCKET_ERROR)
    {
        //int32_t error = GetSocketError();
        return false;
    }

    // Set the interface for outgoing multicast packets on the socket
    if (mAddrInfo->ai_addr->sa_family == AF_INET)
    {
        // Set the options for V4
        optLevel = IPPROTO_IP;
        option   = IP_MULTICAST_IF;
        optVal = (char*)&multicastInterface;
        optLen = sizeof(multicastInterface);
    }
    else if (mAddrInfo->ai_addr->sa_family == AF_INET6)
    {
        // Set the options for V6
        optLevel = IPPROTO_IPV6;
        option   = IPV6_MULTICAST_IF;
        optVal = (char*)&multicastInterfaceIpv6;
        optLen = sizeof(multicastInterfaceIpv6);
    }
    
    if (SetSockOpt(optLevel, option, optVal, optLen) == SOCKET_ERROR)
    {
        return false;
    }

    // Set multicast TTL
    if (mAddrInfo->ai_addr->sa_family == AF_INET)
    {
        // Set the options for V4
        optLevel = IPPROTO_IP;
        option   = IP_MULTICAST_TTL;
        optVal   = (char*)&timeToLive;
        optLen   = sizeof(timeToLive);
    }
    else if (mAddrInfo->ai_addr->sa_family == AF_INET6)
    {
        // Set the options for V6
        optLevel = IPPROTO_IPV6;
        option   = IPV6_MULTICAST_HOPS;
        optVal   = (char*)&timeToLive;
        optLen   = sizeof(timeToLive);
    }
    
    if (SetSockOpt(optLevel, option, optVal, optLen) == SOCKET_ERROR)
    {
        return false;
    }

    // Set multicast loopback
    int32_t nLoopback = loopback;
    if (mAddrInfo->ai_addr->sa_family == AF_INET)
    {
        // Set the options for V4
        optLevel = IPPROTO_IP;
        option   = IP_MULTICAST_LOOP;
        optVal   = (char*)&nLoopback;
        optLen   = sizeof(nLoopback);

        if (SetSockOpt(optLevel, option, optVal, optLen) == SOCKET_ERROR)
        {
            return false;
        }
    }
    else if (mAddrInfo->ai_addr->sa_family == AF_INET6)
    {
        // Set the options for IPv6
        optLevel = IPPROTO_IPV6;
        option   = IPV6_MULTICAST_LOOP;
        optVal   = (char*)&nLoopback;
        optLen   = sizeof(nLoopback);

        if (SetSockOpt(optLevel, option, optVal, optLen) == SOCKET_ERROR)
        {
            return false;
        }
    }

    return true;
}

bool UdpSocket::LeaveMulticastGroup(const std::string& multicastAddress, const std::string& interfaceAddress)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (!IsValid() || mAddrInfo == nullptr || multicastAddress.length() == 0)
    {
        return false;
    }

    std::shared_ptr<addrinfo> interfaceAddressInfo = nullptr;
    if (!interfaceAddress.empty())
    {
        interfaceAddressInfo = NetworkUtility::GetAddressInfo(interfaceAddress, 0, SOCK_DGRAM, IPPROTO_UDP, true);
    }

    in_addr multicastInterface = { 0 };
    in6_addr multicastInterfaceIpv6 = { 0 };

    if (interfaceAddressInfo != nullptr)
    {
        if (interfaceAddressInfo->ai_addr->sa_family == AF_INET)
        {
            inet_pton(AF_INET, interfaceAddress.c_str(), &multicastInterface);
        }
        else
        {
            inet_pton(AF_INET6, interfaceAddress.c_str(), &multicastInterfaceIpv6);
        }
    }

    uint32_t anyMulticastInterfaceIndex = INADDR_ANY;
    
    // Set socket multicast options
    char* optVal = NULL;
    int32_t optLevel = 0;
    int32_t option = 0;
    int32_t optLen = 0;

    ip_mreq mreqv4;    
    ipv6_mreq mreqv6;

    if (mAddrInfo->ai_addr->sa_family == AF_INET)
    {
        // Setup the v4 option values and ip_mreq structure
        inet_pton(AF_INET, multicastAddress.c_str(), &mreqv4.imr_multiaddr);
        memcpy(&mreqv4.imr_interface, &multicastInterface, sizeof(multicastInterface));

        optLevel = IPPROTO_IP;
        option   = IP_DROP_MEMBERSHIP;
        optVal   = (char*)&mreqv4;
        optLen   = sizeof(mreqv4);

        // Leave the multicast group
        if (SetSockOpt(optLevel, option, optVal, optLen) == SOCKET_ERROR)
        {
            return false;
        }

        return true;
    }
    else if (mAddrInfo->ai_addr->sa_family == AF_INET6)
    {
        // Setup the v6 option values and ipv6_mreq structure
        inet_pton(AF_INET6, multicastAddress.c_str(), &mreqv6.ipv6mr_multiaddr.s6_addr);
        if (interfaceAddress.empty())
        {
            mreqv6.ipv6mr_interface = anyMulticastInterfaceIndex;
        }
        else
        {
            std::shared_ptr<addrinfo> interfaceAddressInfo = NetworkUtility::GetAddressInfo(interfaceAddress, 0, SOCK_DGRAM, IPPROTO_UDP, true);
            mreqv6.ipv6mr_interface = ((sockaddr_in6*)interfaceAddressInfo->ai_addr)->sin6_scope_id;
        }

// Define IPV6_DROP_MEMBERSHIP for FreeBSD and Mac OS X
#ifndef IPV6_DROP_MEMBERSHIP
#define IPV6_DROP_MEMBERSHIP IPV6_LEAVE_GROUP
#endif

        optLevel = IPPROTO_IPV6;
        option   = IPV6_DROP_MEMBERSHIP;
        optVal   = (char*)&mreqv6;
        optLen   = sizeof(mreqv6);
    }
    else
    {
        return false;
    }

    // Leave the multicast group
    if (SetSockOpt(optLevel, option, optVal, optLen) == SOCKET_ERROR)
    {
        return false;
    }

    return true;
}

bool UdpSocket::Bind(const std::string& hostAddress, uint16_t port)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    // Close previous socket
    Detach();
    
    if (!CreateSocketFromAddress(hostAddress, port, SOCK_DGRAM, IPPROTO_UDP, true))
    {
        return false;
    }

    // Set SO_REUSEADDR on the socket to true (1)
    int32_t optval = 1;
    SetSockOpt(SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
    
    // Set SO_LINGER on the socket to false (0):
    linger lingerval = { 0 };
    SetSockOpt(SOL_SOCKET, SO_LINGER, (char*)&lingerval, sizeof(lingerval));

    // Setup the UDP listening socket
    int32_t result = bind(mSocket, mAddrInfo->ai_addr, (int32_t)mAddrInfo->ai_addrlen);
    if (result == SOCKET_ERROR)
    {
        int32_t error = GetSocketError();
        THROW(SocketException, error, hostAddress, port);
    }

    // Get the host name and port number
    GetSocketAddress();

#ifdef _MSC_VER
    // If sending a datagram using the sendto function results in an "ICMP port unreachable"
    // response and the select function is set for readfds, the program returns 1 and the subsequent
    // call to the recvfrom function does not work with a WSAECONNRESET (10054) error response.
    // In Microsoft Windows NT 4.0, this situation causes the select function to block or time out.
    DWORD dwBytesReturned = 0;
    BOOL bNewBehavior = FALSE;

    // Controls whether UDP PORT_UNREACHABLE messages are reported. Set to TRUE to enable reporting.
    // Set to FALSE to disable reporting.
    // IOCTL: SIO_UDP_CONNRESET
    result = WSAIoctl(mSocket, SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), nullptr, 0, &dwBytesReturned, nullptr, nullptr);
    if (result == SOCKET_ERROR)
    {
        // WSAIoctl(SIO_UDP_CONNRESET) Error
        int32_t error = GetSocketError();
        THROW(SocketException, error, HostAddress.cref(), HostPort.cref());
    }
#endif

    mConnected();
    return true;
}
