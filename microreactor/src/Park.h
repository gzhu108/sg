#pragma once
#ifndef microreactor_Park
#define microreactor_Park

#include <iterator>
#include <unordered_map>
#include "Parkable.h"


namespace microreactor
{
    template <typename T>
    class Park
    {
    public:
        typedef T ParkingSpaceNumber;

        Park()
        {
        }

        virtual ~Park()
        {
        }

    public:
        virtual std::shared_ptr<Parkable<ParkingSpaceNumber>> Add(ParkingSpaceNumber parkingSpace, std::shared_ptr<Parkable<ParkingSpaceNumber>> parkable)
        {
            ScopeLock<decltype(mLock)> scopeLock(mLock);

            auto oldParkable = Remove(parkingSpace);

            mParkables[parkingSpace] = parkable;
            parkable->AssignParkingSpace(parkingSpace);
            return oldParkable;
        }

        virtual std::shared_ptr<Parkable<ParkingSpaceNumber>> Remove(ParkingSpaceNumber parkingSpace)
        {
            ScopeLock<decltype(mLock)> scopeLock(mLock);

            auto parking = mParkables.find(parkingSpace);
            if (parking == mParkables.end())
            {
                return nullptr;
            }

            auto parkable = parking->second;
            parkable->UnassignParkingSpace();
            mParkables.erase(parking);

            return parkable;
        }

        virtual std::shared_ptr<Parkable<ParkingSpaceNumber>> Get(ParkingSpaceNumber parkingSpace)
        {
            ScopeLock<decltype(mLock)> scopeLock(mLock);

            auto parking = mParkables.find(parkingSpace);
            if (parking == mParkables.end())
            {
                return nullptr;
            }

            return parking->second;
        }

        virtual bool GetAll(std::vector<std::shared_ptr<Parkable<ParkingSpaceNumber>>>& parkables)
        {
            ScopeLock<decltype(mLock)> scopeLock(mLock);

            parkables.clear();
            std::transform(mParkables.begin(), mParkables.end(), std::back_inserter(parkables), [](const auto& parkable)
            {
                return parkable.second;
            });

            return !parkables.empty();
        }

    protected:
        mutable std::recursive_mutex mLock;
        std::unordered_map<ParkingSpaceNumber, std::shared_ptr<Parkable<ParkingSpaceNumber>>> mParkables;
    };
}


#endif // microreactor_Park
