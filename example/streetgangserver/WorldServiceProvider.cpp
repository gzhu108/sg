#include "WorldServiceProvider.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


std::vector<streetgangapi::Point<float>> WorldServiceProvider::mItems;

WorldServiceProvider::WorldServiceProvider()
{
    if (mItems.empty())
    {
        mItems.reserve(100);

        for (float x = 0.0f; x < 1.0f; x += 0.1f)
        {
            for (float y = 0.0f; y < 1.0f; y += 0.1f)
            {
                mItems.emplace_back(Point<float>(x, y, 0.0f));
            }
        }
    }
}

WorldServiceProvider::~WorldServiceProvider()
{
}

uint64_t WorldServiceProvider::GetItemsInRect(const streetgangapi::Rectangle<float>& rect, std::vector<streetgangapi::Point<float>>& items)
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
