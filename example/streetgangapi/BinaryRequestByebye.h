#pragma once
#ifndef streetgangapi_BinaryRequestByebye
#define streetgangapi_BinaryRequestByebye

#include "RequestByebye.h"


namespace streetgangapi
{
    class BinaryRequestByebye : public RequestByebye
    {
    public:
        BinaryRequestByebye();
        virtual ~BinaryRequestByebye();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangapi_BinaryRequestByebye
