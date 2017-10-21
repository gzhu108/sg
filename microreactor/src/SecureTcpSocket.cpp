#include "SecureTcpSocket.h"
#include "Exception.h"

using namespace sg::microreactor;


SecureTcpSocket::SecureTcpSocket()
    : mContext(nullptr)
    , mSsl(nullptr)
{
}

SecureTcpSocket::~SecureTcpSocket()
{
    if (mContext)
    {
        SSL_CTX_free(mContext);
        mContext = nullptr;
    }

    if (mSsl != nullptr)
    {
        SSL_shutdown(mSsl);
        SSL_free(mSsl);
        mSsl = nullptr;
    }
}

std::shared_ptr<TcpSocket> SecureTcpSocket::Accept(const std::chrono::milliseconds& timeout)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (mContext == nullptr)
    {
        return nullptr;
    }

    std::shared_ptr<TcpSocket> client = TcpSocket::Accept(timeout);
    if (client == nullptr)
    {
        return nullptr;
    }

    auto secureClient = std::make_shared<SecureTcpSocket>();
    secureClient->Swap(*client);
    secureClient->mSsl = SSL_new(mContext);
    SSL_set_fd(secureClient->mSsl, (int)secureClient->mSocket);

    if (SSL_accept(secureClient->mSsl) <= 0)
    {
        return nullptr;
    }

    return secureClient;
}

bool SecureTcpSocket::Connect(const std::string& address, uint16_t port, const std::chrono::milliseconds& timeout)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (!TcpSocket::Connect(address, port, timeout))
    {
        return false;
    }

    return true;
}

bool SecureTcpSocket::Receive(char* buffer, int32_t length, int32_t& bytesReceived)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    if (mSsl == nullptr || buffer == nullptr || length <= 0)
    {
        return false;
    }

    bytesReceived = SSL_read(mSsl, buffer, length);
    return bytesReceived > 0;
}

bool SecureTcpSocket::Send(const char* buffer, int32_t length, int32_t& bytesSent)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    if (mSsl == nullptr || buffer == nullptr || length <= 0)
    {
        return false;
    }

    bytesSent = SSL_write(mSsl, buffer, length);
    return bytesSent > 0;
}

bool SecureTcpSocket::ConfigureContext(const std::string& privateKey, const std::string& certificate)
{
    const SSL_METHOD* method = SSLv23_server_method();
    mContext = SSL_CTX_new(method);
    if (mContext == nullptr)
    {
        return false;
    }

    SSL_CTX_set_ecdh_auto(mContext, 1);

    // Set the key and cert
    if (SSL_CTX_use_certificate_file(mContext, certificate.c_str(), SSL_FILETYPE_PEM) <= 0)
    {
        SSL_CTX_free(mContext);
        mContext = nullptr;
        return false;
    }

    if (SSL_CTX_use_PrivateKey_file(mContext, privateKey.c_str(), SSL_FILETYPE_PEM) <= 0 )
    {
        SSL_CTX_free(mContext);
        mContext = nullptr;
        return false;
    }

    return true;
}
