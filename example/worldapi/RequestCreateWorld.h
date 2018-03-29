#pragma once
#ifndef worldapi_RequestCreateWorld
#define worldapi_RequestCreateWorld

#include "MessageBase.h"


namespace worldapi
{
    class RequestCreateWorld : public MessageBase
    {
    public:
        RequestCreateWorld();
        virtual ~RequestCreateWorld();
        
        PROPERTY(WorldName, std::string, "New World");

    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // worldapi_RequestCreateWorld
