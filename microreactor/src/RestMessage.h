#pragma once
#ifndef microreactor_RestMessage
#define microreactor_RestMessage

#include <string>
#include <vector>
#include "Message.h"
#include "Connection.h"
#include "HttpHeader.h"


namespace microreactor
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
        static std::shared_ptr<RestMessage> Parse(std::shared_ptr<RestMessage> parent, std::shared_ptr<std::string> buffer);
        virtual bool Send(Connection& connection);
        virtual std::shared_ptr<RestMessage> CreateMessage() = 0;

    public:
        RestMessageType mMessageType;
        std::string mVersion = "HTTP/1.1";
        std::vector<HttpHeader> mHeaders;
        HttpChunk mBody;
        std::vector<std::shared_ptr<RestMessage>> mChunks;
        bool mChunkCompleted = false;
        std::shared_ptr<std::string> mRawMessage;
    };
}


#endif // microreactor_RestMessage
