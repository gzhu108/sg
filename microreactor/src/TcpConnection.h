#pragma once
#ifndef sg_microreactor_TcpConnection
#define sg_microreactor_TcpConnection

#include <vector>
#include "Connection.h"
#include "TcpSocket.h"

#define DEFAULT_TCP_CONNECTION_BUFFER_SIZE 4096


namespace sg { namespace microreactor
{ 
    class TcpConnection : public Connection
    {
    public:
        TcpConnection(std::shared_ptr<TcpSocket> socket, std::shared_ptr<Profile> profile);
        virtual ~TcpConnection();
        
        PROPERTY_SET(ReceiveBufferSize, uint32_t, DEFAULT_TCP_CONNECTION_BUFFER_SIZE, TcpConnection::SetReceiveBufferSize);
        
    public:
        virtual std::string GetPeerAddress() const override;
        virtual uint16_t GetPeerPort() const override;

        virtual bool DataReady() override;
        virtual uint64_t Receive(char* buffer, int32_t length) override;
        virtual uint64_t Send(const char* buffer, int32_t length) override;

        virtual bool IsClosed() override;

    protected:
        virtual bool Close() override;
        virtual bool EnsureConnection();
        void SetReceiveBufferSize(PROPERTY_TYPE(ReceiveBufferSize)& propertyValue, const uint32_t& value);
        
    protected:
        std::shared_ptr<TcpSocket> mSocket;
        bool mServerConnection;
    };
}}


#endif // sg_microreactor_TcpConnection
