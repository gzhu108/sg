#pragma once
#ifndef streetgangapi_PBRequestByebye
#define streetgangapi_PBRequestByebye

#include "PBMessage.h"
#include "RequestByebye.h"


namespace streetgangapi
{
    class PBRequestByebye : public PBMessage<RequestByebye>
    {
    public:
        PBRequestByebye();
        virtual ~PBRequestByebye();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        bool Decode(google::protobuf::io::CodedInputStream& codedInputStream);
    };
}


#endif // streetgangapi_PBRequestByebye
