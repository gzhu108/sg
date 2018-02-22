#pragma once
#ifndef sg_microreactor_SocketBuffer
#define sg_microreactor_SocketBuffer

#include <sstream>
#include "Connection.h"


namespace sg { namespace microreactor
{
    class SocketBuffer : public std::stringbuf
    {
    public:
        SocketBuffer(Connection& connection);
        virtual ~SocketBuffer();

    public:
        virtual uint64_t Receive();
        virtual uint64_t Send();

    protected:
        Connection& mConnection;
    };
}}


#endif // sg_microreactor_SocketBuffer
