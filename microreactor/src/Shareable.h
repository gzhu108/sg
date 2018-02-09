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

        template <typename T>
        std::shared_ptr<T> SharedFromThis()
        {
            return std::static_pointer_cast<T>(shared_from_this());
        }
    };
}}

#endif // sg_microreactor_Shareable
