#pragma once
#ifndef microreactor_HttpParser
#define microreactor_HttpParser

#include "Common.h"


namespace microreactor
{
    struct HttpChunk
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

    template<typename T>
    void SetHttpBody(std::shared_ptr<std::string> body, T& message)
    {
        if (body != nullptr)
        {
            message.mRawMessage = body;
            message.mBody.mOffset = message.mRawMessage->data();
            message.mBody.mLength = message.mRawMessage->length();
        }
    }

    template<typename T>
    void SetHttpBody(const std::string& body, T& message)
    {
        SetHttpBody(std::make_shared<std::string>(body), message);
    }
}


#endif // microreactor_HttpParser
