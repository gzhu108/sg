#pragma once
#ifndef sg_service_MSearchMessage
#define sg_service_MSearchMessage

#include "RestMessage.h"
#include "RestRequest.h"


namespace sg { namespace service
{
    class MSearchMessage : public sg::microreactor::RestMessage
    {
    public:
        MSearchMessage();
        MSearchMessage(std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~MSearchMessage();

        static const char S[];
        static const char HOST[];
        static const char MAN[];
        static const char MX[];
        static const char ST[];

    public:
        bool GetHeader(const std::string& name, std::string& value) const;
        void AddHeader(const std::string& name, const std::string& value);

    protected:
        virtual bool Decode(std::shared_ptr<sg::microreactor::RestRequest> request);

    protected:
        std::map< std::string, std::string> mHeaders;
    };
}}

#endif // sg_service_MSearchMessage
