#pragma once
#ifndef sg_microreactor_Profile
#define sg_microreactor_Profile

#include "Property.h"


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
    };
}}


#endif // sg_microreactor_Profile
