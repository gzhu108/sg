#pragma once
#ifndef simplewebserver_GetFileReactor
#define simplewebserver_GetFileReactor

#include "Microreactor.h"
#include "GetFileMessage.h"


namespace simplewebserver
{
    class GetFileReactor : public sg::microreactor::RestReactor<GetFileMessage>
    {
    public:
        GetFileReactor(std::shared_ptr<sg::microreactor::Connection> connection, std::shared_ptr<GetFileMessage> request);
        virtual ~GetFileReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // simplewebserver_GetFileReactor
