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
        virtual bool JoinMulticastGoup(const std::string& multicastAddress, const std::string& interfaceAddress = {}, bool loopback = false, uint32_t timeToLive = 1);
        virtual bool LeaveMulticastGroup(const std::string& multicastAddress, const std::string& interfaceAddress);
        virtual bool Bind(const std::string& hostName, uint16_t port);
    };
}}


#endif //sg_microreactor_UdpSocket
