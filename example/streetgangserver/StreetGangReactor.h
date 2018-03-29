#pragma once
#ifndef streetgangserver_StreetGangReactor
#define streetgangserver_StreetGangReactor

#include "StreetGangResponder.h"


namespace streetgangserver
{
    class StreetGangReactor
    {
    public:
        StreetGangReactor(std::shared_ptr<streetgangapi::StreetGangResponder> responder);
        virtual ~StreetGangReactor();

    public:
        virtual std::shared_ptr<streetgangapi::StreetGangResponder> GetResponder() { return mResponder; }

    protected:
        std::shared_ptr<streetgangapi::StreetGangResponder> mResponder;
    };
}


#endif // streetgangserver_StreetGangReactor
