#pragma once
#ifndef sg_microreactor_RestRequest
#define sg_microreactor_RestRequest

#include <string>
#include <vector>
#include "Connection.h"
#include "HttpHeader.h"


namespace sg { namespace microreactor
{
    struct RestRequest
    {
        std::shared_ptr<std::string> mRawMessage;
        std::string mMethod;
        std::string mUri;
        std::string mVersion;
        std::vector<HttpHeader> mHeaders;
        HttpChunk mBody;
        std::vector<RestRequest> mChunks;
        bool mChunkCompleted = false;

        virtual bool Parse(std::shared_ptr<std::string> message);
        virtual bool Send(Connection& connection);
        virtual bool FlushToBuffer(std::string& buffer);
        virtual bool FlushToStream(std::ostream& stream);
    };
}}


#endif // sg_microreactor_RestRequest
