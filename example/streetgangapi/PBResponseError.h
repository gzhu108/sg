#pragma once
#ifndef streetgangapi_PBResponseError
#define streetgangapi_PBResponseError

#include "ResponseError.h"
#include "PBMessage.h"


namespace streetgangapi
{
    class PBResponseError : public PBMessage<ResponseError>
    {
    public:
        PBResponseError();
        virtual ~PBResponseError();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangapi_PBResponseError
