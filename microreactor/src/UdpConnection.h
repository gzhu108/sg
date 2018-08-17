#pragma once
#ifndef sg_microreactor_UdpConnection
#define sg_microreactor_UdpConnection

#include <vector>
#include "Connection.h"
#include "UdpSocket.h"

#define DEFAULT_UDP_CONNECTION_BUFFER_SIZE 1440


namespace sg { namespace microreactor
{
    class UdpConnection : public Connection
    {
    public:
        UdpConnection(std::shared_ptr<UdpSocket> socket, std::shared_ptr<Profile> profile);
        virtual ~UdpConnection();
        
    public:
        virtual std::string GetPeerName() const override;
        virtual uint16_t GetPeerPort() const override;

        virtual void SetPeerName(const std::string& peerName) override;
        virtual void SetPeerPort(uint16_t peerPort) override;

        virtual bool DataReady() override;
        virtual uint64_t Receive(char* buffer, int32_t length) override;
        virtual uint64_t Send(const char* buffer, int32_t length) override;
        
        virtual bool IsClosed() override;

    protected:
        virtual bool Close() override;
        
    protected:
        std::shared_ptr<UdpSocket> mSocket;
        bool mDataRetrieved = false;
    };
}}


#endif // sg_microreactor_UdpConnection
