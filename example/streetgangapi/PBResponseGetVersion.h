#pragma once
#ifndef streetgangapi_PBResponseGetVersion
#define streetgangapi_PBResponseGetVersion

#include "PBMessage.h"
#include "ResponseGetVersion.h"


namespace streetgangapi
{
    class PBResponseGetVersion : public PBMessage<ResponseGetVersion>
    {
    public:
        PBResponseGetVersion();
        virtual ~PBResponseGetVersion();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        bool Decode(google::protobuf::io::CodedInputStream& codedInputStream);
    };
}


#endif // streetgangapi_PBResponseGetVersion
