#pragma once
#ifndef sg_microreactor_HttpParser
#define sg_microreactor_HttpParser

#include "Common.h"


namespace sg { namespace microreactor
{
    struct HttpData
    {
        const char* mOffset = nullptr;
        size_t mLength = 0;
    };

    struct HttpHeader
    {
        std::string mName;
        std::string mValue;
        
        HttpHeader() {}
        HttpHeader(const std::string& name, const std::string& value)
            : mName(name)
            , mValue(value)
        {
        }
    };

    struct HttpChunk : HttpData
    {
        std::shared_ptr<std::string> mRawData;
    };
}}


#endif // sg_microreactor_HttpParser
