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

void Task::SetExecuteTime(const std::chrono::time_point<std::chrono::high_resolution_clock>& executeTime)
{
    mExecuteTime = executeTime;
}

bool Task::Execute()
{
    if (mStatus != TaskStatus::Created && mStatus != TaskStatus::Scheduled)
    {
        return false;
    }

    if (mExecuteTime > std::chrono::high_resolution_clock::now())
    {
        // The task is not ready to run
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
