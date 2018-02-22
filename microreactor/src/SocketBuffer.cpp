#include "SocketBuffer.h"

using namespace sg::microreactor;


SocketBuffer::SocketBuffer(Connection& connection)
    : mConnection(connection) 
{
}

SocketBuffer::~SocketBuffer()
{
}

uint64_t SocketBuffer::Receive()
{
    return mConnection.Receive(egptr(), 1000);
}

uint64_t SocketBuffer::Send()
{
    int32_t length = (int32_t)std::distance(pbase(), epptr());
    if (length == 0)
    {
        return 0;
    }

    return mConnection.Send(pbase(), length);
}
