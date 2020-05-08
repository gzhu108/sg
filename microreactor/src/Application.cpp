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

bool Application::Run()
{
    for (auto& service : mServices)
    {
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

bool Application::Run(std::shared_ptr<Listener> listener)
{
    return Run(std::make_shared<Service>(listener));
}

bool Application::Run(std::shared_ptr<Service> service)
{
    mServices.clear();
    mServices.emplace_back(service);
    return Run();
}

bool Application::Run(const std::vector<std::shared_ptr<Listener>>& listeners)
{
    mServices.clear();

    for (auto& listener : listeners)
    {
        mServices.emplace_back(std::make_shared<Service>(listener));
    }

    return Run();
}

bool Application::Run(const std::vector<std::shared_ptr<Service>>& services)
{
    mServices.clear();
    std::copy(services.begin(), services.end(), std::back_inserter(mServices));
    return Run();
}
