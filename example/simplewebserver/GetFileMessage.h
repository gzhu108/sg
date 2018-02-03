#pragma once
#ifndef simplewebserver_GetFileMessage
#define simplewebserver_GetFileMessage

#include "Microreactor.h"


namespace simplewebserver
{
    class GetFileMessage : public sg::microreactor::RestMessage
    {
    public:
        GetFileMessage(std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~GetFileMessage();
    };
}


#endif // simplewebserver_GetFileMessage
