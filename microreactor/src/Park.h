#pragma once
#ifndef sg_microreactor_Park
#define sg_microreactor_Park

#include <unordered_map>
#include "Parkable.h"


namespace sg { namespace microreactor
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
            if (!mParkables.empty())
            {
                for (const auto& parking : mParkables)
                {
                    parkables.push_back(parking.second);
                }
            }

            return !parkables.empty();
        }

    protected:
        mutable std::recursive_mutex mLock;
        std::unordered_map<ParkingSpaceNumber, std::shared_ptr<Parkable<ParkingSpaceNumber>>> mParkables;
    };
}}


#endif // sg_microreactor_Park
