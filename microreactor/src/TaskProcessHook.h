#pragma once
#ifndef microreactor_TaskProcessHook
#define microreactor_TaskProcessHook

#include "Task.h"


namespace microreactor
{
    class TaskProcessHook
    {
    public:
        Signal<TaskPtr>& Preprocess = mPreprocess;
        Signal<TaskPtr>& Postprocess = mPostprocess;
        
    protected:
        Emittable<TaskPtr> mPreprocess;
        Emittable<TaskPtr> mPostprocess;
    };
}


#endif // microreactor_TaskProcessHook
