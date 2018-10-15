#pragma once
#ifndef worldserver_WorldProvider
#define worldserver_WorldProvider

#include <vector>
#include "World.h"


namespace worldserver
{
    class WorldProvider : public worldapi::World
    {
    public:
        WorldProvider();
        virtual ~WorldProvider();
        
    public:
        void InsertItem(const worldapi::Point<float>& item);
        const worldapi::Rectangle<float>& GetWorldBoundary() { return mWorldBoundary; }
        uint64_t GetItems(std::vector<worldapi::Point<float>>& items);
        uint64_t GetItemsInRect(const worldapi::Rectangle<float>& rect, std::vector<worldapi::Point<float>>& items);
    };
}

#endif // worldserver_WorldProvider
