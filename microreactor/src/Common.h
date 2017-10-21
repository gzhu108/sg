#pragma once
#ifndef sg_microreactor_Common
#define sg_microreactor_Common

#include "MemoryTrack.h"
#include <cstdint>
#include <cstring>
#include <errno.h>
#include <sstream>
#include <memory>
#include <stdio.h>
#include "LoggerSingleton.h"
#include "Property.h"

#ifdef _MSC_VER
#include <Winsock2.h>
#include <Windows.h>
#undef min
#undef max
#endif

#ifdef SendMessage
#undef SendMessage
#endif

#ifndef _countof
#define _countof(_array) (sizeof(_array) / sizeof((_array)[0]))
#endif

#ifdef _MSC_VER
#define FMT_INT64 "%I64d"
#define FMT_UINT64 "%I64u"
#else
#define FMT_INT64 "%ld"
#define FMT_UINT64 "%lu"
#endif

#define FIND_CMD(_cmds, _arg) (&_cmds[_countof(_cmds)] != std::find_if_not(_cmds, &_cmds[_countof(_cmds)], std::bind(strcmp, std::placeholders::_1, _arg)))


namespace sg { namespace microreactor
{
#ifdef _MSC_VER
    inline int32_t GetSocketError()
    {
        int32_t error = WSAGetLastError();
        return error == WSAETIMEDOUT ? EAGAIN : error;
    }
#else
    inline int32_t GetSocketError()
    {
        return errno;
    }
#endif
}}

// workaround for std::to_string not defined
namespace std
{
    template<typename T>
    string to_string(const T& val)
    {
        ostringstream s;
        s << val;
        return s.str();
    }
}


#endif // sg_microreactor_Common
