#pragma once
#ifndef streetgangapi_PBResponseCreateWorld
#define streetgangapi_PBResponseCreateWorld

#include "PBMessage.h"
#include "ResponseCreateWorld.h"


namespace streetgangapi
{
    class PBResponseCreateWorld : public PBMessage<ResponseCreateWorld>
    {
    public:
        PBResponseCreateWorld();
        virtual ~PBResponseCreateWorld();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        bool Decode(google::protobuf::io::CodedInputStream& codedInputStream);
    };
}


#endif // streetgangapi_PBResponseCreateWorld
