#pragma once
#ifndef streetgangapi_SessionId
#define streetgangapi_SessionId

#include "Common.h"


namespace streetgangapi
{
    typedef uint64_t SessionId;
    const SessionId DefaultSessionId = (SessionId)(0);
    const SessionId InvalidSessionId = (SessionId)(-1);

    //typedef std::string SessionId;
    //const SessionId DefaultSessionId = "DefaultSession";
    //const SessionId InvalidSessionId = "InvalidSession";
}


#endif // streetgangapi_SessionId
