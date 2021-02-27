#pragma once
#ifndef microreactor_Cancelable
#define microreactor_Cancelable

#include "Common.h"


namespace microreactor
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
}


#endif // microreactor_Cancelable
