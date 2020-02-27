#include "StreetGangBinaryService.h"
#include "SecureTcpSocket.h"
#include "TcpListener.h"
#include "ConfigurationSingleton.h"
#include "BinarySerializer.h"
#include "BinaryRequestByebye.h"
#include "StreetGangBinaryDispatcher.h"
#include "DiscoveryClient.h"
#include "Uuid.h"

//CHECK_MEMORY_INIT

#define TASK_LATENCY_THRESHOLD ((double)0.01)

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;
using namespace sg::service;


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

StreetGangBinaryService::StreetGangBinaryService()
    : mTaskLatencyThreshold(TASK_LATENCY_THRESHOLD)
{
    auto configuration = ConfigurationSingleton::GetConfiguration();
    if (configuration != nullptr)
    {
        configuration->GetValue("TaskLatencyThreshold", mTaskLatencyThreshold);
    }
}

StreetGangBinaryService::~StreetGangBinaryService()
{
}

bool StreetGangBinaryService::Start()
{
    // Hook to task process
    //auto& taskProcessHook = GET_TASK_PROCESS_HOOK();
    //taskProcessHook.Preprocess.Connect(std::bind(&StreetGangPBService::OnTaskPreprocess, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));
    //taskProcessHook.Postprocess.Connect(std::bind(&StreetGangPBService::OnTaskPostprocess, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));

    return Service::Start();
}

bool StreetGangBinaryService::Stop()
{
    if (mListener != nullptr)
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
                SendAllConnections(stream);
            }
        }

        // Disconnect to task process
        //auto& taskProcessHook = GET_TASK_PROCESS_HOOK();
        //taskProcessHook.Preprocess.Disconnect(reinterpret_cast<uintptr_t>(this));
        //taskProcessHook.Postprocess.Disconnect(reinterpret_cast<uintptr_t>(this));

        // Disable hot-config
        auto configuration = ConfigurationSingleton::GetConfiguration();
        if (configuration != nullptr)
        {
            configuration->ValueUpdated.Disconnect(mConfigurationConnectionId);
        }
    }

    return Service::Stop();
}

bool StreetGangBinaryService::Initialize()
{
    auto configuration = ConfigurationSingleton::GetConfiguration();
    if (configuration == nullptr)
    {
        return false;
    }

    mConfigurationConnectionId = configuration->ValueUpdated.Connect(std::bind(&StreetGangBinaryService::Restart, this));

    uint32_t listenTimeout = 30;
    uint32_t receiveTimeout = 30;
    uint32_t sendTimeout = 100;

    configuration->GetValue("ListenTimeout", listenTimeout);
    configuration->GetValue("ReceiveTimeout", receiveTimeout);
    configuration->GetValue("SendTimeout", sendTimeout);

    auto dispatcher = std::make_shared<StreetGangBinaryDispatcher>();

    std::string protocol = "tcp";
    configuration->GetValue("Protocol", protocol);
    dispatcher->Protocol.set(protocol);

    std::string address = ANY_HOST;
    configuration->GetValue("ServiceAddress", address);
    dispatcher->Address.set(address);

    uint16_t port = 8390;
    configuration->GetValue("ServicePort", port);
    dispatcher->Port.set(port);

    auto socket = std::make_shared<SecureTcpSocket>();
    socket->ConfigureSslContext(SSLv23_server_method(), "cert/Server.key", "cert/Server.cer", VerifyPeer);
    socket->LoadSslContextVerifyLocations("cert/ChainCA.cer", "");
    //auto socket = std::make_shared<TcpSocket>();
    mListener = std::make_shared<TcpListener>(socket, dispatcher);
    LOG("SECURE TCP HOST: %s", mListener->Name->c_str());

    mListener->ListenTimeout.set(std::chrono::milliseconds(listenTimeout));
    mListener->ReceiveTimeout.set(std::chrono::milliseconds(receiveTimeout));
    mListener->SendTimeout.set(std::chrono::milliseconds(sendTimeout));

    return true;
}

void StreetGangBinaryService::SetResponseTime(std::shared_ptr<MessageBase> response)
{
    std::chrono::duration<double> responseTime = std::chrono::high_resolution_clock::now() - response->GetCreationTime();

    if (responseTime.count() > mTaskLatencyThreshold)
    {
        LOG("[Request=%d] [Latency=%f]", response->Id.cref(), responseTime.count());
    }
}

void StreetGangBinaryService::OnTaskPreprocess(const TaskPtr& task)
{
    //mTaskProcessTime[task.get()] = std::chrono::high_resolution_clock::now();
    uint64_t count = TaskManagerSingleton::GetTaskManager()->GetQueueDepth();
    LOG("Current queue depth = " FMT_UINT64 "", count);
}

void StreetGangBinaryService::OnTaskPostprocess(const TaskPtr& task)
{
    std::chrono::time_point<std::chrono::high_resolution_clock> processTimeEnd = std::chrono::high_resolution_clock::now();

    auto taskItr = mTaskProcessTime.find(task.get());
    if (taskItr != mTaskProcessTime.end())
    {
        std::chrono::duration<double> processTimeUsed = processTimeEnd - taskItr->second;
        mTaskProcessTime.erase(taskItr);

        if (processTimeUsed.count() > mTaskLatencyThreshold)
        {
            LOG("[Task=%s] [Latency=%f]", task->Name->c_str(), processTimeUsed.count());
        }
    }
    else
    {
        LOG("Task (%p) not found", task.get());
    }
}
