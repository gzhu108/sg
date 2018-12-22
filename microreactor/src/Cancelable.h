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
        virtual ~Cancelable() {}

        Signal<void>& Completed = mCompleted;
        Signal<void>& Canceled = mCanceled;

    public:
        virtual void Cancel() = 0;

    protected:
        Emittable<void> mCompleted;
        Emittable<void> mCanceled;
    };
} }


#endif // sg_microreactor_Cancelable
