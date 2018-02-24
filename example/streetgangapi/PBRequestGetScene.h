#pragma once
#ifndef streetgangapi_PBRequestGetScene
#define streetgangapi_PBRequestGetScene

#include "PBMessage.h"
#include "RequestGetScene.h"


namespace streetgangapi
{
    class PBRequestGetScene : public PBMessage<RequestGetScene>
    {
    public:
        PBRequestGetScene();
        virtual ~PBRequestGetScene();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        bool Decode(google::protobuf::io::CodedInputStream& codedInputStream);
    };
}


#endif // streetgangapi_PBRequestGetScene
