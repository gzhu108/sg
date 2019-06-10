#include "Application.h"
#include <signal.h>
#include "TaskManagerSingleton.h"
#include "Service.h"

using namespace sg::microreactor;


static std::recursive_mutex gApplicationLock;
static std::shared_ptr<Application> gApplicationInstance;

volatile bool terminateSignal = false;
void SingalHandler(int type)
{
    terminateSignal = true;
    DESTROY_TASK_MANAGER();
}

Application& Application::Context()
{
    std::lock_guard<decltype(gApplicationLock)> scopeLock(gApplicationLock);

    if (gApplicationInstance == nullptr)
    {
        gApplicationInstance.reset(new Application());
    }

    return *gApplicationInstance;
}

Application::Application()
{
    // Set signal handlers for graceful termination
    signal(SIGABRT, SingalHandler);
    signal(SIGINT, SingalHandler);
    signal(SIGTERM, SingalHandler);

#ifndef _MSC_VER
    signal(SIGPIPE, SIG_IGN);
#endif
}

Application::~Application()
{
}

bool Application::Run(std::vector<std::shared_ptr<Endpoint>> endpoints)
{
    for (auto& endpoint : endpoints)
    {
        auto service = std::make_shared<Service>(endpoint);
        mServices.emplace_back(service);
        if (!service->Start())
        {
            return false;
        }
    }

    // Block until ctrl-C is pressed
    START_BLOCKING_TASK_LOOP();

    for (auto& service : mServices)
    {
        service->Stop();
    }

    // Cancell all tasks
    CANCEL_ALL_TASKS_AND_DESTROY_TASK_MANAGER();

    return true;
}
