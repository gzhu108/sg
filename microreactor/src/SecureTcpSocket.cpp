#include "SecureTcpSocket.h"
#include "Exception.h"
#include <openssl/bio.h>

#ifndef SSL_CTX_set_ecdh_auto
#define SSL_CTX_set_ecdh_auto(dummy, onoff) ((void)0)
#endif

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

    if (mContext == nullptr)
    {
        return false;
    }

    if (!TcpSocket::Connect(address, port, timeout))
    {
        return false;
    }

    mSsl = SSL_new(mContext); //create new SSL connection state
    SSL_set_fd(mSsl, (int)mSocket); // attach the socket descriptor

    // perform the connection
    if (SSL_connect(mSsl) <= 0)
    {
        Detach();
        return false;
    }

    ShowCerts();

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

bool SecureTcpSocket::ConfigureServerContext(const std::string& privateKey, const std::string& certificate)
{
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

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

bool SecureTcpSocket::ConfigureClientContext()
{
    OpenSSL_add_all_algorithms(); // Load cryptos, et.al.
    SSL_load_error_strings(); //Bring in and register error messages

    const SSL_METHOD* method = SSLv23_client_method(); // Create new client-method instance
    mContext = SSL_CTX_new(method);	//Create new context
    if (mContext == NULL)
    {
        return false;
    }

    return true;
}

void SecureTcpSocket::ShowCerts()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    if (mSsl == nullptr)
    {
        return;
    }

    ERR_load_BIO_strings();
    ERR_load_crypto_strings();

    BUF_MEM* bptr = nullptr;
    BIO* outbio = BIO_new(BIO_s_mem());
    BIO_get_mem_ptr(outbio, &bptr);

    X509* cert = SSL_get_peer_certificate(mSsl);
    if (cert != nullptr)
    {
        BIO_printf(outbio, "Certificate Subject:\n");
        X509_NAME_print_ex(outbio, X509_get_subject_name(cert), 0, 0);
        BIO_printf(outbio, "\n");

        BIO_printf(outbio, "Certificate Issuer:\n");
        X509_NAME_print_ex(outbio, X509_get_issuer_name(cert), 0, 0);
        BIO_printf(outbio, "\n");

        std::string s(bptr->data, bptr->length);
        LOG("%s\n", s.c_str());

        X509_free(cert);
    }
    else
    {
        LOG("No certificate.\n");
    }

    BIO_free(outbio);
}
