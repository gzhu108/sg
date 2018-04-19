#pragma once
#ifndef sg_microreactor_SecureTcpSocket
#define sg_microreactor_SecureTcpSocket

#include "TcpSocket.h"
#include <openssl/ssl.h>
#include <openssl/err.h>


namespace sg { namespace microreactor
{
    class SecureTcpSocket : public TcpSocket
    {
    public:
        SecureTcpSocket();
        virtual ~SecureTcpSocket();

    public:
        virtual std::shared_ptr<TcpSocket> Accept(const std::chrono::milliseconds& timeout) override;
        virtual bool Connect(const std::string& address, uint16_t port, const std::chrono::milliseconds& timeout) override;
    
        virtual bool Receive(char* buffer, int32_t length, int32_t& bytesReceived) override;
        virtual bool Send(const char* buffer, int32_t length, int32_t& bytesSent) override;

        virtual bool ConfigureSslContext(const SSL_METHOD* method, const std::string& privateKey, const std::string& certificate, SSL_verify_cb verifyPeer);
        virtual bool LoadSslContextVerifyLocations(const std::string& caFile, const std::string& caPath);
        virtual bool AddContextClientCa(X509* caCert);
        static int VerifyPeer(int preverifyOk, X509_STORE_CTX* context);

    protected:
        virtual void ShowCerts();

    protected:
        SSL_CTX* mContext;
        SSL* mSsl;
    };
}}


#endif //sg_microreactor_SecureTcpSocket
