#pragma once
#ifndef sg_microreactor_RestResponse
#define sg_microreactor_RestResponse

#include <string>
#include <vector>
#include "Connection.h"
#include "HttpHeader.h"


namespace sg { namespace microreactor
{
    struct RestResponse
    {
        std::string mVersion = "HTTP/1.1";
        uint32_t mStatusCode = 200;
        std::string mStatusText = "OK";
        std::vector<HttpHeader> mHeaders;
        std::string mBody;

        virtual bool Send(Connection& connection);
        virtual bool FlushToBuffer(std::string& buffer);
        virtual bool FlushToStream(std::ostream& stream);

        static bool SendWith(Connection& connection, uint32_t statusCode, const std::string& statusText, const std::vector<HttpHeader>& headers = {}, const std::string& body = {}, bool connectionClosed = false);
        
        inline static bool SendErrorWith(Connection& connection, uint32_t statusCode, const std::string& statusText, const std::vector<HttpHeader>& headers = {}, const std::string& body = {})
        {
            return SendWith(connection, statusCode, statusText, headers, body, true);
        }
    };
}}


#endif // sg_microreactor_RestResponse
