#pragma once
#ifndef sg_microreactor_TaskProcessHook
#define sg_microreactor_TaskProcessHook

#include "Task.h"


namespace sg { namespace microreactor
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
}}


#endif // sg_microreactor_TaskProcessHook
