#pragma once
#ifndef streetgangapi_PBRequestByebye
#define streetgangapi_PBRequestByebye

#include "RequestByebye.h"


namespace streetgangapi
{
    class PBRequestByebye : public RequestByebye
    {
    public:
        PBRequestByebye();
        virtual ~PBRequestByebye();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangapi_PBRequestByebye
