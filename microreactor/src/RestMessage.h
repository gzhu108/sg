#pragma once
#ifndef sg_microreactor_RestMessage
#define sg_microreactor_RestMessage

#include <string>
#include <vector>
#include "Message.h"
#include "Connection.h"
#include "HttpHeader.h"


namespace sg { namespace microreactor
{
    class RestMessage : public Message
    {
    public:
        enum RestMessageType
        {
            UnknownType = 0,
            Request,
            Response,
        };

        RestMessage(const RestMessageType& messageType);
        virtual ~RestMessage();

    public:
        static std::shared_ptr<RestMessage> Parse(std::shared_ptr<RestMessage> parent, std::shared_ptr<std::string> message);
        virtual bool Send(Connection& connection);
        virtual bool FlushToBuffer(std::string& buffer) = 0;
        virtual bool FlushToStream(std::ostream& stream);
        virtual std::shared_ptr<RestMessage> CreateMessage() = 0;

    public:
        RestMessageType mMessageType;
        std::string mVersion;
        std::vector<HttpHeader> mHeaders;
        HttpChunk mBody;
        std::vector<std::shared_ptr<RestMessage>> mChunks;
        bool mChunkCompleted = false;
        std::shared_ptr<std::string> mRawMessage;
    };
}}


#endif // sg_microreactor_RestMessage
