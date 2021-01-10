#include "StreetGangBinaryClient.h"
#include "ConfigurationSingleton.h"
#include "SecureTcpSocket.h"
#include "TcpConnection.h"
#include "BinarySerializer.h"
#include "Uuid.h"
#include "StreetGangBinaryClientDispatcher.h"
#include "BinaryResponseError.h"
#include "BinaryRequestByebye.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


static int VerifyPeer(int preverifyOk, X509_STORE_CTX* context)
{
#if 1

    char buf[256];
    X509* err_cert = X509_STORE_CTX_get_current_cert(context);
    int err = X509_STORE_CTX_get_error(context);
    int depth = X509_STORE_CTX_get_error_depth(context);
    X509_NAME_oneline(X509_get_subject_name(err_cert), buf, 256);

    if (!preverifyOk)
    {
        printf("verify error:num=%d:%s:depth=%d:%s\n", err, X509_verify_cert_error_string(err), depth, buf);
    }

    return 1;

#else

    struct mydata_t
    {
        int verbose_mode;
        int verify_depth;
        int always_continue;
    };
    int mydata_index = 0;

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
        X509_NAME_oneline(X509_get_issuer_name(err_cert), buf, 256);
        printf("issuer= %s\n", buf);
    }

    if (mydata->always_continue)
    {
        return 1;
    }

    return preverifyOk;
#endif
}

StreetGangBinaryClient::StreetGangBinaryClient(const std::string& protocol, const std::string& hostAddress, uint16_t port)
{
    auto dispatcher = std::make_shared<StreetGangBinaryClientDispatcher>();
    dispatcher->Protocol.set(protocol);
    dispatcher->Address.set(hostAddress);
    dispatcher->Port.set(port);

    auto configuration = ConfigurationSingleton::GetConfiguration();
    if (configuration)
    {

        bool useSecureSocket = false;
        configuration->GetValue("UseSecureSocket", useSecureSocket);

        std::shared_ptr<TcpSocket> socket;
        if (useSecureSocket)
        {
            auto secureSocket = std::make_shared<SecureTcpSocket>();
            secureSocket->ConfigureSslContext(SSLv23_client_method(), "cert/Client.key", "cert/Client.cer", VerifyPeer);
            secureSocket->LoadSslContextVerifyLocations("cert/ChainCA.cer", "");
            socket = std::static_pointer_cast<TcpSocket>(secureSocket);
        }
        else
        {
            socket = std::make_shared<TcpSocket>();
        }

        auto connection = std::make_shared<TcpConnection>(socket, dispatcher);
        Initialize(connection);
    }
}

StreetGangBinaryClient::~StreetGangBinaryClient()
{
    if (mConnection != nullptr && !mConnection->IsClosed())
    {
        auto message = std::make_shared<BinaryRequestByebye>();
        message->TrackId.set(Uuid::GenerateUuid().ToString());

        std::stringstream messageStream;
        if (message->Encode(messageStream))
        {
            BinarySerializer serializer;
            std::stringstream stream;
            uint64_t length = GetStreamSize(messageStream);
            if (serializer.Write(message->Id.cref(), stream) &&
                serializer.Write(length, stream) &&
                serializer.Write(messageStream, stream))
            {
                // Send byebye message
                mConnection->Send(stream);
            }
        }
    }
}
