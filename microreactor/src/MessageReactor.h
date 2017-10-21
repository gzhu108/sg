#pragma once
#ifndef sg_microreactor_MessageReactor
#define sg_microreactor_MessageReactor

#include "Reactor.h"
#include "Connection.h"


namespace sg { namespace microreactor
{
    template <typename T, typename MessageEncoderType>
    class MessageReactor : public Reactor
    {
    public:
        typedef T MessageType;

        MessageReactor(Connection& connection, std::shared_ptr<MessageType> input)
            : Reactor(connection)
        {
            mInput = std::static_pointer_cast<Message>(input);
        }

        virtual ~MessageReactor() {}

    public:
        virtual std::shared_ptr<MessageType> InputMessage()
        {
            return std::static_pointer_cast<MessageType>(mInput);
        }

        void SetMessageEncoder(std::shared_ptr<MessageEncoderType> messageEncoder)
        {
            mMessageEncoder = messageEncoder;
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
                // Send message
                uint64_t sent = mConnection.Send(stream);
                if (sent > 0)
                {
                    return true;
                }
            }

            auto peerName = mConnection.GetPeerName();
            auto peerPort = mConnection.GetPeerPort();
            LOG("[TrackId=%s] Fail to send message [Connection=%s:%u]", message->TrackId->c_str(), peerName.c_str(), peerPort);

            return false;
        }

    protected:
        std::shared_ptr<MessageEncoderType> mMessageEncoder;
    };

    template <typename T>
    class MessageReactor<T, void> : public Reactor
    {
    public:
        typedef T MessageType;

        MessageReactor(Connection& connection, std::shared_ptr<MessageType> input)
            : Reactor(connection)
        {
            mInput = std::static_pointer_cast<Message>(input);
        }

        virtual ~MessageReactor() {}

    public:
        virtual std::shared_ptr<MessageType> InputMessage()
        {
            return std::static_pointer_cast<MessageType>(mInput);
        }
    };
}}


#endif // sg_microreactor_MessageReactor
