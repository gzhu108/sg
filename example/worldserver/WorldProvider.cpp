#include "WorldProvider.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace worldserver;


WorldProvider::WorldProvider()
    : mWorldBoundary(0.0, 0.0, 0.0, 0.0)
{
    if (mItems.empty())
    {
        mItems.reserve(100);

        for (float x = 0.0f; x < 1.0f; x += 0.1f)
        {
            for (float y = 0.0f; y < 1.0f; y += 0.1f)
            {
                InsertItem(Point<float>(x, y, 0.0f));
            }
        }
    }
}

WorldProvider::~WorldProvider()
{
}

void WorldProvider::InsertItem(const worldapi::Point<float>& item)
{
    if (item.mX < mWorldBoundary.mX)
    {
        mWorldBoundary.mX = item.mX;
    }
    else if (item.mX - mWorldBoundary.mX > mWorldBoundary.mW)
    {
        mWorldBoundary.mW = item.mX - mWorldBoundary.mX;
    }

    if (item.mY < mWorldBoundary.mY)
    {
        mWorldBoundary.mY = item.mY;
    }
    else if (item.mY - mWorldBoundary.mY > mWorldBoundary.mH)
    {
        mWorldBoundary.mH = item.mY - mWorldBoundary.mY;
    }

    mItems.emplace_back(item);
}

uint64_t WorldProvider::GetItems(std::vector<worldapi::Point<float>>& items)
{
    items = mItems;
    return items.size();
}

uint64_t WorldProvider::GetItemsInRect(const worldapi::Rectangle<float>& rect, std::vector<worldapi::Point<float>>& items)
{
    items.clear();
    
    for (auto& item : mItems)
    {
        if (item.mX >= rect.mX &&
            item.mX < (rect.mX + rect.mW) &&
            item.mY >= rect.mY &&
            item.mY < (rect.mY + rect.mH))
        {
            items.emplace_back(item);
        }
    }
    
    return items.size();
}
