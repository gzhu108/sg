#include "SecureTcpSocket.h"
#include "Exception.h"
#include <openssl/bio.h>
#include <openssl/x509_vfy.h>

#ifndef SSL_CTX_set_ecdh_auto
#define SSL_CTX_set_ecdh_auto(dummy, onoff) ((void)0)
#endif

using namespace microreactor;


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

std::shared_ptr<TcpSocket> SecureTcpSocket::Accept()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (mContext == nullptr)
    {
        return nullptr;
    }

    std::shared_ptr<TcpSocket> client = TcpSocket::Accept();
    if (client == nullptr)
    {
        return nullptr;
    }

    auto secureClient = std::make_shared<SecureTcpSocket>();
    secureClient->Swap(*client);
    secureClient->mSsl = SSL_new(mContext);
    int result = SSL_set_fd(secureClient->mSsl, (int)secureClient->mSocket);

    result = SSL_accept(secureClient->mSsl);
    if (result <= 0)
    {
        LOG("SSL_accept() error = %d\n", result);
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

    if (!TcpSocket::Connect(address, port))
    {
        return false;
    }

    mSsl = SSL_new(mContext); //create new SSL connection state
    int result = SSL_set_fd(mSsl, (int)mSocket); // attach the socket descriptor

    // perform the connection
    result = SSL_connect(mSsl);
    while (result <= 0)
    {
        result = SSL_get_error(mSsl, result);

        if (result == SSL_ERROR_WANT_READ)
        {
        }
        else if (result == SSL_ERROR_WANT_WRITE)
        {
            if (!SendWait(timeout))
            {
                LOG("SSL_connect() write timeout\n");
                Detach();
                return false;
            }
        }
        else
        {
            LOG("SSL_connect() error = %d\n", result);
            Detach();
            return false;
        }

        result = SSL_connect(mSsl);
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

bool SecureTcpSocket::ConfigureSslContext(const SSL_METHOD* method, const std::string& privateKey, const std::string& certificate, SSL_verify_cb verifyPeer)
{
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    if (method == nullptr)
    {
        return false;
    }

    mContext = SSL_CTX_new(method);
    if (mContext == nullptr)
    {
        return false;
    }

    SSL_CTX_set_ecdh_auto(mContext, 1);

    // Set the key and cert
    if (!certificate.empty() && SSL_CTX_use_certificate_chain_file(mContext, certificate.c_str()) <= 0)
    {
        SSL_CTX_free(mContext);
        mContext = nullptr;
        return false;
    }

    if (!privateKey.empty() && SSL_CTX_use_PrivateKey_file(mContext, privateKey.c_str(), SSL_FILETYPE_PEM) <= 0 )
    {
        SSL_CTX_free(mContext);
        mContext = nullptr;
        return false;
    }

    if (SSL_CTX_check_private_key(mContext) <= 0)
    {
        SSL_CTX_free(mContext);
        mContext = nullptr;
        return false;
    }

    if (verifyPeer != nullptr)
    {
        SSL_CTX_set_verify(mContext, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, verifyPeer);
    }

    return true;
}

bool SecureTcpSocket::LoadSslContextVerifyLocations(const std::string& caFile, const std::string& caPath)
{
    if (mContext == nullptr)
    {
        return false;
    }

    int result = SSL_CTX_load_verify_locations(mContext, caFile.empty() ? nullptr : caFile.c_str(), caPath.empty() ? nullptr : caPath.c_str());
    if (result > 0)
    {
        //result = SSL_CTX_set_client_CA_list(mContext, SSL_get_client_CA_list)
    }

    return result > 0;
}

bool SecureTcpSocket::AddContextClientCa(X509* caCert)
{
    if (mContext == nullptr)
    {
        return false;
    }

    int result = SSL_CTX_add_client_CA(mContext, caCert);
    return result > 0;
}

int SecureTcpSocket::VerifyPeer(int preverifyOk, X509_STORE_CTX* context)
{
    char buf[256];
    X509* err_cert = X509_STORE_CTX_get_current_cert(context);
    int err = X509_STORE_CTX_get_error(context);
    int depth = X509_STORE_CTX_get_error_depth(context);
    X509_NAME_oneline(X509_get_subject_name(err_cert), buf, 256);

    if (!preverifyOk)
    {
        LOG("Certificate verification error: num=%d:%s:depth=%d:%s\n", err, X509_verify_cert_error_string(err), depth, buf);
    }

    return preverifyOk;
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
