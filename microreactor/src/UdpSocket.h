#pragma once
#ifndef sg_microreactor_UdpSocket
#define sg_microreactor_UdpSocket

#include "Socket.h"
#include "NetworkUtility.h"


namespace sg { namespace microreactor
{
    class UdpSocket : public Socket
    {
    public:
        UdpSocket();
        explicit UdpSocket(SOCKET socket);
        virtual ~UdpSocket();
        
    public:
        virtual bool JoinMulticastGoup(const std::string& multicastAddress, std::shared_ptr<addrinfo> interfaceInfo = nullptr, bool loopback = false, uint32_t timeToLive = 1);
        virtual bool LeaveMulticastGroup(const std::string& multicastAddress, std::shared_ptr<addrinfo> interfaceInfo = nullptr);
        
        virtual bool Bind(const std::string& hostName, uint16_t port);
        virtual bool Create(const std::string& address, uint16_t port);
    };
}}


#endif //sg_microreactor_UdpSocket
