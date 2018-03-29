#pragma once
#ifndef streetgangserver_WorldServiceProvider
#define streetgangserver_WorldServiceProvider

#include <vector>
#include "SessionId.h"
#include "streetgangapi/Point.h"
#include "streetgangapi/Rectangle.h"


namespace streetgangserver
{
    class WorldServiceProvider
    {
    public:
        WorldServiceProvider();
        virtual ~WorldServiceProvider();

        typedef streetgangapi::SessionId SessionId;
        PROPERTY(Id, SessionId);
        
    public:
        uint64_t GetItemsInRect(const streetgangapi::Rectangle<float>& rect, std::vector<streetgangapi::Point<float>>& items);
       
    protected:
        static std::vector<streetgangapi::Point<float>> mItems;
    };
}

#endif // streetgangserver_WorldServiceProvider
