#pragma once
#ifndef sg_microreactor_Parkable
#define sg_microreactor_Parkable

#include "Common.h"
#include "Shareable.h"


namespace sg { namespace microreactor
{
    class Parkable : public Shareable
    {
    public:
        Parkable();
        virtual ~Parkable();

    public:
        virtual bool IsParked() { return mParking != 0; }
        virtual void AssignParking(uintptr_t parking) { mParking = parking; }
        virtual uintptr_t GetParking() const { return mParking; }

    protected:
        uintptr_t mParking = 0;
    };
}}


#endif // sg_microreactor_Parkable
