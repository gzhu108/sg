#include "Task.h"

using namespace sg::microreactor;


Task::Task()
{
}

Task::Task(std::function<void ()> run)
    : mRun(run)
{
}

Task::~Task()
{
}

bool Task::Execute()
{
    if (mStatus != TaskStatus::Created && mStatus != TaskStatus::Scheduled)
    {
        return false;
    }

    // Check if the task has been canceled
    if (mStatus != TaskStatus::Canceled)
    {
        mStatus = TaskStatus::Running;
        try
        {
            mRun();
        }
        catch (...)
        {
            LOG("Exception: Failed to run task: %s", Name->c_str());
        }
        mStatus = TaskStatus::Completed;
    }

    mCompleted();
    return true;
}
