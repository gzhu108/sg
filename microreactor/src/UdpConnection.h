#pragma once
#ifndef microreactor_UdpConnection
#define microreactor_UdpConnection

#include <vector>
#include "Connection.h"
#include "UdpSocket.h"

#define DEFAULT_UDP_CONNECTION_BUFFER_SIZE 1440


namespace microreactor
{
    class UdpConnection : public Connection
    {
    public:
        UdpConnection(std::shared_ptr<UdpSocket> socket, std::shared_ptr<microreactor::Dispatcher> dispatcher);
        virtual ~UdpConnection();
        
    public:
        virtual std::string GetPeerAddress() const override;
        virtual uint16_t GetPeerPort() const override;

        virtual Connection& SetPeerAddress(const std::string& peerAddress) override;
        virtual Connection& SetPeerPort(uint16_t peerPort) override;

        virtual uint64_t Receive(char* buffer, int length) override;
        virtual uint64_t Send(const char* buffer, int length) override;
        
        virtual bool IsClosed() override;

    protected:
        virtual bool Close() override;
        
    protected:
        std::shared_ptr<UdpSocket> mSocket;
        bool mServerConnection;
        bool mDataRetrieved;
    };
}


#endif // microreactor_UdpConnection
