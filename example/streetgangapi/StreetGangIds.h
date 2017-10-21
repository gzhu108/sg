#pragma once
#ifndef streetgangapi_StreetGangIds
#define streetgangapi_StreetGangIds


namespace streetgangapi
{
    enum class ID : int32_t
    {
        Unknown = 0,
        Byebye = 1,
        Error = -1,

        GetVersionRequest = 1000,
        CreateWorldRequest = 1001,
        GetSceneRequest = 1002,

        GetVersionResponse = 2000,
        CreateWorldResponse = 2001,
        GetSceneResponse = 2002,
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
}

#endif // streetgangapi_StreetGangIds
