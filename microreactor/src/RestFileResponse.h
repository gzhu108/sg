#pragma once
#ifndef sg_microreactor_RestFileResponse
#define sg_microreactor_RestFileResponse

#include <fstream>
#include "RestResponse.h"


namespace sg { namespace microreactor
{
    class RestFileResponse : public RestResponse
    {
    public:
        explicit RestFileResponse(const std::string& path);
        virtual ~RestFileResponse();

    public:
        virtual bool Send(sg::microreactor::Connection& connection) override;

    protected:
        std::ifstream mFileStream;
    };
}}


#endif // sg_microreactor_RestFileResponse
