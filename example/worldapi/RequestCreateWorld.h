#pragma once
#ifndef worldapi_RequestCreateWorld
#define worldapi_RequestCreateWorld

#include "WorldMessage.h"


namespace worldapi
{
    class RequestCreateWorld : public WorldMessage
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
