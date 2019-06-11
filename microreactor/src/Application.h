#pragma once
#ifndef sg_microreactor_Application
#define sg_microreactor_Application

#include <vector>
#include "Service.h"


namespace sg { namespace microreactor
{
    class Application
    {
    public:
        virtual ~Application();

    public:
        static Application& Context();
        virtual bool Run();
        virtual bool Run(const std::vector<std::shared_ptr<Endpoint>>& endpoints);

    protected:
        Application();

    protected:
        std::vector<std::shared_ptr<Service>> mServices;
    };
}}

#endif // sg_microreactor_Microservice
