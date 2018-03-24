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

bool SecureTcpSocket::ConfigureSslContext(const std::string& privateKey, const std::string& certificate, SSL_verify_cb verifyPeer)
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
    if (!certificate.empty() && SSL_CTX_use_certificate_file(mContext, certificate.c_str(), SSL_FILETYPE_PEM) <= 0)
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

    if (verifyPeer != nullptr)
    {
        SSL_CTX_set_verify(mContext, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, verifyPeer);
    }

    return true;
}

int SecureTcpSocket::VerifyPeer(int preverifyOk, X509_STORE_CTX* context)
{
#if 1

    return 1;

#else

    struct mydata_t
    {
        int verbose_mode;
        int verify_depth;
        int always_continue;
    };
    int mydata_index;

    char    buf[256];
    X509   *err_cert;
    int     err, depth;
    SSL    *ssl;
    mydata_t *mydata;

    err_cert = X509_STORE_CTX_get_current_cert(context);
    err = X509_STORE_CTX_get_error(context);
    depth = X509_STORE_CTX_get_error_depth(context);

    /*
    * Retrieve the pointer to the SSL of the connection currently treated
    * and the application specific data stored into the SSL object.
    */
    ssl = (SSL*)X509_STORE_CTX_get_ex_data(context, SSL_get_ex_data_X509_STORE_CTX_idx());
    mydata = (mydata_t*)SSL_get_ex_data(ssl, mydata_index);

    X509_NAME_oneline(X509_get_subject_name(err_cert), buf, 256);

    /*
    * Catch a too long certificate chain. The depth limit set using
    * SSL_CTX_set_verify_depth() is by purpose set to "limit+1" so
    * that whenever the "depth>verify_depth" condition is met, we
    * have violated the limit and want to log this error condition.
    * We must do it here, because the CHAIN_TOO_LONG error would not
    * be found explicitly; only errors introduced by cutting off the
    * additional certificates would be logged.
    */
    if (depth > mydata->verify_depth)
    {
        preverifyOk = 0;
        err = X509_V_ERR_CERT_CHAIN_TOO_LONG;
        X509_STORE_CTX_set_error(context, err);
    }

    if (!preverifyOk)
    {
        printf("verify error:num=%d:%s:depth=%d:%s\n", err,
            X509_verify_cert_error_string(err), depth, buf);
    }
    else if (mydata->verbose_mode)
    {
        printf("depth=%d:%s\n", depth, buf);
    }

    /*
    * At this point, err contains the last verification error. We can use
    * it for something special
    */
    if (!preverifyOk && (err == X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT))
    {
        X509_NAME_oneline(X509_get_issuer_name(context->current_cert), buf, 256);
        printf("issuer= %s\n", buf);
    }

    if (mydata->always_continue)
    {
        return 1;
    }
        
    return preverifyOk;
#endif
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
