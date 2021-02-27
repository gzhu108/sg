#pragma once
#ifndef microreactor_Profile
#define microreactor_Profile

#include "Common.h"


namespace microreactor
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
}


#endif // microreactor_Profile
