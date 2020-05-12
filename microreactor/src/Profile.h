#pragma once
#ifndef sg_microreactor_Profile
#define sg_microreactor_Profile

#include "Common.h"


namespace sg { namespace microreactor
{
    class Profile
    {
    public:
        Profile();
        virtual ~Profile();

        PROPERTY(Protocol, std::string, "tcp");
        PROPERTY(Address, std::string, ANY_HOST);
        PROPERTY(Port, uint16_t, 0);
    };
}}


#endif // sg_microreactor_Profile
