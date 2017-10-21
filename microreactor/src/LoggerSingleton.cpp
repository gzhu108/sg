#include <cstdarg>
#include <algorithm>
#include "LoggerSingleton.h"
#include "ScopeLock.h"

using namespace sg::microreactor;


static std::recursive_mutex gLoggerSingletonLock;
static std::shared_ptr<LoggerSingleton> gLoggerSingletonInstance;
static std::vector<LoggerSingleton::Logger> gLoggerList;


LoggerSingleton::LoggerSingleton()
{
}

LoggerSingleton::~LoggerSingleton()
{
}

LoggerSingleton& LoggerSingleton::Instance()
{
    if (gLoggerSingletonInstance == nullptr)
    {
        gLoggerSingletonInstance.reset(new LoggerSingleton());
    }

    return *gLoggerSingletonInstance;
}

void LoggerSingleton::AddLogger(const Logger& logger)
{
    ScopeLock<decltype(gLoggerSingletonLock)> scopeLock(gLoggerSingletonLock);
    gLoggerList.emplace_back(logger);
}

void LoggerSingleton::Log(const char* text, ...)
{
    if (text == nullptr)
    {
        return;
    }

    va_list args;
    va_start(args, text);
    Log(text, args);
    va_end(args);
}

void LoggerSingleton::Log(const char* text, va_list args)
{
    if (text == nullptr)
    {
        return;
    }

    char buffer[10240];
    buffer[10239] = '\0';

    vsnprintf(buffer, 10240, text, args);
    Log(std::string(buffer));
}

void LoggerSingleton::Log(const std::string& text)
{
    ScopeLock<decltype(gLoggerSingletonLock)> scopeLock(gLoggerSingletonLock);

    if (text.empty() || gLoggerList.empty())
    {
        return;
    }

    for (const auto& logger : gLoggerList)
    {
        try
        {
            logger(text);
        }
        catch(...)
        {
        }
    }
}
