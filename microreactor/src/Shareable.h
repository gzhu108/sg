#pragma once
#ifndef sg_microreactor_Shareable
#define sg_microreactor_Shareable

#include "Common.h"


namespace sg { namespace microreactor
{
    class Shareable : public std::enable_shared_from_this<Shareable>
    {
    public:
        Shareable() {}
        virtual ~Shareable() {}
    };
}}

#endif // sg_microreactor_Shareable
