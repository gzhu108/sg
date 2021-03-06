#pragma once
#ifndef microreactor_LoggerSingleton
#define microreactor_LoggerSingleton

#include <vector>
#include <memory>
#include <string>
#include <functional>

#define GET_LOGGER() microreactor::LoggerSingleton::Instance()

#if NO_LOG
#define LOG(...) (void)0
#else
#define LOG(...) GET_LOGGER().Log(__VA_ARGS__)
#endif


namespace microreactor
{
    class LoggerSingleton
    {
    public:
        virtual ~LoggerSingleton();

    public:
        static LoggerSingleton& Instance();

        typedef std::function<void (const std::string&)> Logger;
        void AddLogger(const Logger& logger);

        void Log(const char* text, ...);
        void Log(const char* text, va_list args);
        void Log(const std::string& text);

    private:
        LoggerSingleton();
    };
}


#endif // microreactor_LoggerSingleton
