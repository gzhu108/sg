#pragma once
#ifndef sg_microreactor_RestRequest
#define sg_microreactor_RestRequest

#include <string>
#include <vector>
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
        HttpData mBody;
        std::vector<HttpChunk> mChunks;
        bool mChunkCompleted = false;

        virtual bool Parse(std::shared_ptr<std::string> message);
    };
}}


#endif // sg_microreactor_RestRequest
