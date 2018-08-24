#pragma once
#ifndef simplewebserver_GetFileMessage
#define simplewebserver_GetFileMessage

#include "Microreactor.h"


namespace simplewebserver
{
    class GetFileMessage : public sg::microreactor::RestRequest
    {
    public:
        GetFileMessage();
        virtual ~GetFileMessage();
    };
}


#endif // simplewebserver_GetFileMessage
