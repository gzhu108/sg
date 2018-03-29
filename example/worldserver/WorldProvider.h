#pragma once
#ifndef worldserver_WorldProvider
#define worldserver_WorldProvider

#include <vector>
#include "WorldId.h"
#include "Point.h"
#include "Rectangle.h"


namespace worldserver
{
    class WorldProvider
    {
    public:
        WorldProvider();
        virtual ~WorldProvider();

        PROPERTY(Id, worldapi::WorldId);
        
    public:
        void InsertItem(const worldapi::Point<float>& item);
        const worldapi::Rectangle<float>& GetWorldBoundary() { return mWorldBoundary; }
        uint64_t GetItems(std::vector<worldapi::Point<float>>& items);
        uint64_t GetItemsInRect(const worldapi::Rectangle<float>& rect, std::vector<worldapi::Point<float>>& items);

    protected:
       
    protected:
        worldapi::Rectangle<float> mWorldBoundary;
        std::vector<worldapi::Point<float>> mItems;
    };
}

#endif // worldserver_WorldProvider
