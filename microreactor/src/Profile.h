#pragma once
#ifndef sg_microreactor_Profile
#define sg_microreactor_Profile

#include "Configuration.h"
#include "Dispatcher.h"


namespace sg { namespace microreactor
{
    class Profile
    {
    public:
        Profile();
        virtual ~Profile();

        PROPERTY(Protocol, std::string);
        PROPERTY(Address, std::string);
        PROPERTY(Port, uint16_t);
        PROPERTY(Configuration, std::shared_ptr<sg::microreactor::Configuration>);
        PROPERTY(Dispatcher, std::shared_ptr<sg::microreactor::Dispatcher>);
    };
}}


#endif // sg_microreactor_Profile
