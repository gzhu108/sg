#pragma once
#ifndef myserver_POSTv1updateRequestContent
#define myserver_POSTv1updateRequestContent

#include <nlohmann/json.hpp>
#include "Microreactor.h"


namespace myserver
{
    class POSTv1updateRequestContent
    {
    public:
        POSTv1updateRequestContent();
        virtual ~POSTv1updateRequestContent();

        PROPERTY(Name, std::string, "");
        PROPERTY(Age, std::string, "18");

    public:
        virtual bool Encode(nlohmann::json& serializer) const;
        virtual bool Decode(const nlohmann::json& serializer);
        virtual bool Write(std::string& buffer) const;
    };
}


#endif // myserver_POSTv1updateRequestContent
