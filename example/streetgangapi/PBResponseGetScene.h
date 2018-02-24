#pragma once
#ifndef streetgangapi_PBResponseGetScene
#define streetgangapi_PBResponseGetScene

#include "PBMessage.h"
#include "ResponseGetScene.h"


namespace streetgangapi
{
    class PBResponseGetScene : public PBMessage<ResponseGetScene>
    {
    public:
        PBResponseGetScene();
        virtual ~PBResponseGetScene();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        bool Decode(google::protobuf::io::CodedInputStream& codedInputStream);
    };
}


#endif // streetgangapi_PBResponseGetScene
