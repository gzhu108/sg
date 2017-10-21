#pragma once
#ifndef sg_microreactor_Cancelable
#define sg_microreactor_Cancelable

#include "Common.h"


namespace sg { namespace microreactor
{
    class Cancelable
    {
    public:
        Cancelable() {}
        virtual ~Cancelable() { mCompleted(); }

        Signal<void>& Completed = mCompleted;

    public:
        virtual void Cancel() = 0;

    protected:
        Emittable<void> mCompleted;
    };
} }


#endif // sg_microreactor_Cancelable
