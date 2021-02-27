#pragma once
#ifndef microreactor_RestFileResponse
#define microreactor_RestFileResponse

#include <fstream>
#include "RestResponse.h"


namespace microreactor
{
    class RestFileResponse : public RestResponse
    {
    public:
        explicit RestFileResponse(const std::string& path);
        virtual ~RestFileResponse();

    public:
        virtual bool Send(Connection& connection) override;

    protected:
        std::ifstream mFileStream;
    };
}


#endif // microreactor_RestFileResponse
