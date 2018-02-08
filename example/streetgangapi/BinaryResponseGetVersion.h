#pragma once
#ifndef streetgangapi_BinaryResponseGetVersion
#define streetgangapi_BinaryResponseGetVersion

#include "ResponseGetVersion.h"


namespace streetgangapi
{
    class BinaryResponseGetVersion : public ResponseGetVersion
    {
    public:
        BinaryResponseGetVersion();
        virtual ~BinaryResponseGetVersion();
        
    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangapi_BinaryResponseGetVersion
