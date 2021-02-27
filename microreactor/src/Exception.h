#pragma once
#ifndef microreactor_Exception
#define microreactor_Exception

#include <stdexcept>
#include <sstream>

#define THROW(_ex, ...) throw _ex(__FILE__, __LINE__, ##__VA_ARGS__)


namespace microreactor
{
    template<typename T>
    struct Exception : T
    {
        std::string mFile;
        uint64_t mLine;
        
        Exception(const std::string& file, uint64_t line, const char* const& what)
            : T(what)
            , mFile(file)
            , mLine(line)
        {
        }
    };
    
    template<>
    struct Exception<std::exception> : std::exception
    {
        std::string mWhat;
        Exception(const std::string& file, uint64_t line, const char* const& what)
        {
            std::stringstream whatStream;
            whatStream << file << "(" << line << ") : " << what;
            mWhat = whatStream.str();
        }
        
#ifdef _MSC_VER
        virtual const char* what() const override
#else
        virtual const char* what() const noexcept override
#endif
        {
            return mWhat.c_str();
        }
    };

    struct OverflowException : Exception<std::overflow_error>
    {
        OverflowException(const std::string& file, uint64_t line)
            : Exception(file, line, "Buffer Overflow")
        {
        }
    };

    struct InvalidDataException : Exception<std::invalid_argument>
    {
        InvalidDataException(const std::string& file, uint64_t line)
            : Exception(file, line, "Invalid Data")
        {
        }
    };

    struct InvalidMessageException : Exception<std::exception>
    {
        InvalidMessageException(const std::string& file, uint64_t line)
            : Exception(file, line, "Invalid Message")
        {
        }
    };

    struct SocketException : Exception<std::exception>
    {
        int32_t mError;
        std::string mName;
        uint16_t mPort;
        SocketException(const std::string& file, uint64_t line, int32_t error, const std::string& name = "", uint16_t port = 0)
            : Exception(file, line, "Socket Error")
            , mError(error)
            , mName(name)
            , mPort(port)
        {
        }
    };

    struct InvalidArgumentException : Exception<std::invalid_argument>
    {
        std::string mValidArguments;
        InvalidArgumentException(const std::string& file, uint64_t line, const std::string& validArgments = "")
            : Exception(file, line, "Invalid Argument")
            , mValidArguments(validArgments)
        {
        }
    };

    struct SignalException : Exception<std::logic_error>
    {
        SignalException(const std::string& file, uint64_t line)
            : Exception(file, line, "Failed to signal")
        {
        }
    };
}


#endif //microreactor_Exception
