#pragma once
#ifndef sg_service_RequestSearch
#define sg_service_RequestSearch

#include <vector>
#include "Serializable.h"
#include "Message.h"


namespace sg { namespace service
{
    class RequestSearch
        : public sg::microreactor::Serializable
        , public sg::microreactor::Message
    {
    public:
        RequestSearch();
        virtual ~RequestSearch();

        PROPERTY(MessageLine, std::string);
        PROPERTY(Endpoint, std::string);
        PROPERTY(Man, std::string);
        PROPERTY(Mx, uint32_t, 0);
        PROPERTY(St, std::string);
        PROPERTY(UserAgent, std::string);

    public:
        virtual bool Serialize(sg::microreactor::Serializer& serializer, std::ostream& stream) const override;
        virtual bool Deserialize(std::istream& stream, sg::microreactor::Serializer& serializer) override;
    };
}}

#endif // sg_service_RequestSearch
