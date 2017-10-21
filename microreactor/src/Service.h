#pragma once
#ifndef sg_microreactor_Server
#define sg_microreactor_Server

#include <vector>
#include "Listener.h"


namespace sg { namespace microreactor
{
    class Service
    {
    public:
        Service();
        virtual ~Service();
        
    public:
        virtual bool Start() = 0;
        virtual bool Stop() = 0;
        virtual void Restart();
    };
}}


#endif // sg_microreactor_Server
