#pragma once
#ifndef microreactor_MemoryTrack
#define microreactor_MemoryTrack

// Memory debugging
#if defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#if defined(_DEBUG)
#ifndef DBG_NEW
#define DBG_NEW new(_NORMAL_BLOCK , __FILE__ , __LINE__)
#define new DBG_NEW
//_CrtSetAllocHook
#endif
#endif
#endif

#if defined(_MSC_VER) && defined(_DEBUG)
#define CHECK_MEMORY_INIT \
    _CrtMemState memStateBegin; \
    _CrtMemState memStateEnd; \
    _CrtMemState memStateDiff;

#define CHECK_MEMORY_BEGIN \
    _CrtMemCheckpoint(&memStateBegin);

#define CHECK_MEMORY_END \
    _CrtMemCheckpoint(&memStateEnd); \
    if (_CrtMemDifference(&memStateDiff, &memStateBegin, &memStateEnd)) \
    { \
        _CrtMemDumpStatistics(&memStateDiff); \
        _CrtMemDumpAllObjectsSince(&memStateBegin); \
    }
#else
#define CHECK_MEMORY_INIT
#define CHECK_MEMORY_BEGIN (void)0;
#define CHECK_MEMORY_END (void)0;
#endif


#endif // microreactor_MemoryTrack
