#pragma once
#ifndef sg_microreactor_RestFactory
#define sg_microreactor_RestFactory

#include "Reactor.h"
#include "RestRequest.h"


namespace sg { namespace microreactor
{
    class RestFactory
    {
    public:
        RestFactory(const std::string& method, const std::string& path, const std::string& contentType = "ContentType: */*")
            : mMethod(method)
            , mPath(path)
            , mContentType(contentType)
        {
        }

        virtual std::shared_ptr<sg::microreactor::Reactor> CreateReactor(std::shared_ptr<RestRequest> request, sg::microreactor::Connection& connection) = 0;

    public:
        const std::string mMethod; // GET, POST, PUT, DELETE, etc.
        const std::string mPath; // Uri path
        const std::string mContentType; // text/plain, text/html, application/json, etc.
    };
}}


#endif // sg_microreactor_RestFactory
