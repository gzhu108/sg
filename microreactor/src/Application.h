#pragma once
#ifndef microreactor_Application
#define microreactor_Application

#include <vector>
#include "Service.h"


namespace microreactor
{
    class Application
    {
    public:
        virtual ~Application();

    public:
        static Application& Context();
        virtual bool Run();
        virtual bool Run(std::shared_ptr<Listener> listener);
        virtual bool Run(std::shared_ptr<Service> service);
        virtual bool Run(const std::vector<std::shared_ptr<Listener>>& listeners);
        virtual bool Run(const std::vector<std::shared_ptr<Service>>& services);

    protected:
        Application();

    protected:
        std::vector<std::shared_ptr<Service>> mServices;
    };
}

#endif // microreactor_Microservice
