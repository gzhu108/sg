#pragma once
#ifndef sg_microreactor_Api
#define sg_microreactor_Api

#include "Connection.h"


namespace sg { namespace microreactor
{
    template<typename T>
    class Api
    {
    public:
        typedef T MessageEncoderType;

        Api(Connection& connection, std::shared_ptr<MessageEncoderType> messageEncoder)
            : mConnection(connection)
            , mMessageEncoder(messageEncoder)
        {
        }

    protected:
        template <typename SendMessageType>
        bool SendMessage(std::shared_ptr<SendMessageType> message)
        {
            if (message == nullptr || mMessageEncoder == nullptr || mConnection.IsClosed())
            {
                return false;
            }

            // Create a new stream for sending message
            std::stringstream stream;
            if (mMessageEncoder->EncodeMessage(message, stream))
            {
                uint64_t sent = mConnection.Send(stream);
                if (sent > 0)
                {
                    // Register the message with the dispatcher
                    mConnection.RegisterMessage(message);
                    return true;
                }
            }

            return false;
        }
        
    protected:
        Connection& mConnection;
        std::shared_ptr<MessageEncoderType> mMessageEncoder;
    };
}}

#endif // sg_microreactor_Api
