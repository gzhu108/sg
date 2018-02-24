#pragma once
#ifndef streetgangapi_PBRequestCreateWorld
#define streetgangapi_PBRequestCreateWorld

#include "PBMessage.h"
#include "RequestCreateWorld.h"


namespace streetgangapi
{
    class PBRequestCreateWorld : public PBMessage<RequestCreateWorld>
    {
    public:
        PBRequestCreateWorld();
        virtual ~PBRequestCreateWorld();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        bool Decode(google::protobuf::io::CodedInputStream& codedInputStream);
    };
}


#endif // streetgangapi_PBRequestCreateWorld
