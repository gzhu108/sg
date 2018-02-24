#pragma once
#ifndef streetgangapi_PBRequestGetVersion
#define streetgangapi_PBRequestGetVersion

#include "PBMessage.h"
#include "RequestGetVersion.h"


namespace streetgangapi
{
    class PBRequestGetVersion : public PBMessage<RequestGetVersion>
    {
    public:
        PBRequestGetVersion();
        virtual ~PBRequestGetVersion();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        bool Decode(google::protobuf::io::CodedInputStream& codedInputStream);
    };
}


#endif // streetgangapi_PBRequestGetVersion
