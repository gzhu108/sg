#pragma once
#ifndef worldapi_WorldId
#define worldapi_WorldId

#include "Common.h"


namespace worldapi
{
    enum class ID : int32_t
    {
        Unknown = 0,
        Error = -1,

        CreateWorldRequest = 1001,
        GetWorldRequest = 1002,

        CreateWorldResponse = 2001,
        GetWorldResponse = 2002,
    };

    enum class ResultCode : int32_t
    {
        Unknown = 0,
        Success = 200,
        ErrorBadRequest = 400,
        ErrorUnauthorized = 401,
        ErrorForbidden = 403,
        ErrorNotFound = 404,
        ErrorInteranlServerError = 500,
        ErrorNotImplemented = 501,
        ErrorServiceUnavailable = 503,
        ErrorTimeout = 504,
    };

    static const char MARKER = 0xbc;

    typedef uint64_t WorldId;
    static const WorldId DefaultWorldId = (WorldId)(0);
    static const WorldId InvalidWorldId = (WorldId)(-1);
}


#endif // worldapi_WorldId
