#include "Park.h"

using namespace sg::microreactor;


Park Park::mParkingLot;


Park::Park()
{
}

Park::~Park()
{
}

Park& Park::ParkingLot()
{
    return mParkingLot;
}

bool Park::Add(uintptr_t parking, std::shared_ptr<Parkable> parkable)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    auto range = mParkables.equal_range(parking);
    auto i = range.first;
    while(i != range.second)
    {
        if (i->second == parkable)
        {
            break;
        }
        i++;
    }

    if (i == range.second)
    {
        mParkables.emplace(parking, parkable);
        parkable->AssignParking(parking);
        return true;
    }

    return false;
}

uint64_t Park::Remove(uintptr_t parking, std::vector<std::shared_ptr<Parkable>>& parkables)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    parkables.clear();
    auto range = mParkables.equal_range(parking);

    for (auto i = range.first; i != range.second; i++)
    {
        i->second->AssignParking(0);
        parkables.emplace_back(i->second);
    }

    mParkables.erase(range.first, range.second);
    return parkables.size();
}

uint64_t Park::Get(uintptr_t parking, std::vector<std::shared_ptr<Parkable>>& parkables)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    parkables.clear();
    auto range = mParkables.equal_range(parking);

    for (auto i = range.first; i != range.second; i++)
    {
        parkables.emplace_back(i->second);
    }

    return parkables.size();
}
