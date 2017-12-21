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



#if 0
/* ssl_client.c
*
* Copyright (c) 2000 Sean Walton and Macmillan Publishers.  Use may be in
* whole or in part in accordance to the General Public License (GPL).
*
* THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*/

/*****************************************************************************/
/*** ssl_client.c                                                          ***/
/***                                                                       ***/
/*** Demonstrate an SSL client.                                            ***/
/*****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define FAIL    -1

/*---------------------------------------------------------------------*/
/*--- OpenConnection - create socket and connect to server.         ---*/
/*---------------------------------------------------------------------*/
int OpenConnection(const char *hostname, int port)
{
    int sd;
    struct hostent *host;
    struct sockaddr_in addr;

    if ((host = gethostbyname(hostname)) == NULL)
    {
        perror(hostname);
        abort();
    }
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if (connect(sd, &addr, sizeof(addr)) != 0)
    {
        close(sd);
        perror(hostname);
        abort();
    }
    return sd;
}

/*---------------------------------------------------------------------*/
/*--- InitCTX - initialize the SSL engine.                          ---*/
/*---------------------------------------------------------------------*/
SSL_CTX* InitCTX(void)
{
    SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();		/* Load cryptos, et.al. */
    SSL_load_error_strings();			/* Bring in and register error messages */
    method = SSLv2_client_method();		/* Create new client-method instance */
    ctx = SSL_CTX_new(method);			/* Create new context */
    if (ctx == NULL)
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

/*---------------------------------------------------------------------*/
/*--- ShowCerts - print out the certificates.                       ---*/
/*---------------------------------------------------------------------*/
void ShowCerts(SSL* ssl)
{
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl);	/* get the server's certificate */
    if (cert != NULL)
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);							/* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);							/* free the malloc'ed string */
        X509_free(cert);					/* free the malloc'ed certificate copy */
    }
    else
        printf("No certificates.\n");
}

/*---------------------------------------------------------------------*/
/*--- main - create SSL context and connect                         ---*/
/*---------------------------------------------------------------------*/
int main(int count, char *strings[])
{
    SSL_CTX *ctx;
    int server;
    SSL *ssl;
    char buf[1024];
    int bytes;
    char *hostname, *portnum;

    if (count != 3)
    {
        printf("usage: %s <hostname> <portnum>\n", strings[0]);
        exit(0);
    }
    hostname = strings[1];
    portnum = strings[2];

    ctx = InitCTX();
    server = OpenConnection(hostname, atoi(portnum));
    ssl = SSL_new(ctx);						/* create new SSL connection state */
    SSL_set_fd(ssl, server);				/* attach the socket descriptor */
    if (SSL_connect(ssl) == FAIL)			/* perform the connection */
        ERR_print_errors_fp(stderr);
    else
    {
        char *msg = "Hello???";

        printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
        ShowCerts(ssl);								/* get any certs */
        SSL_write(ssl, msg, strlen(msg));			/* encrypt & send message */
        bytes = SSL_read(ssl, buf, sizeof(buf));	/* get reply & decrypt */
        buf[bytes] = 0;
        printf("Received: \"%s\"\n", buf);
        SSL_free(ssl);								/* release connection state */
    }
    close(server);									/* close socket */
    SSL_CTX_free(ctx);								/* release context */
}
#endif


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
