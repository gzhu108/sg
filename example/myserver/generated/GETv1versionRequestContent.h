#pragma once
#ifndef myserver_GETv1versionRequestContent
#define myserver_GETv1versionRequestContent

#include <nlohmann/json.hpp>
#include "Microreactor.h"


namespace myserver
{
    class GETv1versionRequestContent
    {
    public:
        GETv1versionRequestContent();
        virtual ~GETv1versionRequestContent();

        PROPERTY(Version, std::string, "1.0.0.0");

    public:
        virtual bool Encode(nlohmann::json& serializer) const;
        virtual bool Decode(const nlohmann::json& serializer);
        virtual bool Write(std::string& buffer) const;
    };
}


#endif // myserver_GETv1versionRequestContent
