#pragma once
#ifndef bouncer_BouncerProfile
#define bouncer_BouncerProfile

#include "Profile.h"


namespace bouncer
{
    class BouncerProfile : public sg::microreactor::Profile
    {
    public:
        BouncerProfile();
        virtual ~BouncerProfile();

        PROPERTY(TargetName, std::string);
        PROPERTY(TargetPort, uint16_t);
    };
}


#endif // bouncer_BouncerProfile
