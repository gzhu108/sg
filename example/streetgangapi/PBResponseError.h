#pragma once
#ifndef streetgangapi_PBResponseError
#define streetgangapi_PBResponseError

#include "PBMessage.h"
#include "ResponseError.h"


namespace streetgangapi
{
    class PBResponseError : public PBMessage<ResponseError>
    {
    public:
        PBResponseError();
        virtual ~PBResponseError();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        bool Decode(google::protobuf::io::CodedInputStream& codedInputStream);
    };
}


#endif // streetgangapi_PBResponseError
