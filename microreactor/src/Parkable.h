#pragma once
#ifndef microreactor_Parkable
#define microreactor_Parkable

#include "Common.h"
#include "Shareable.h"


namespace microreactor
{
    template <typename T>
    class Parkable : public Shareable
    {
    public:
        typedef T ParkingSpaceNumber;

        Parkable()
        {
        }

        virtual ~Parkable()
        {
        }

    public:
        virtual bool IsParked()
        {
            return mParked;
        }

        virtual void AssignParkingSpace(ParkingSpaceNumber parkingSpace)
        {
            mParkingSpace = parkingSpace;
            mParked = true;
        }

        virtual void UnassignParkingSpace()
        {
            mParked = false;
        }

        virtual ParkingSpaceNumber GetParkingSpace() const
        {
            return mParkingSpace;
        }

    protected:
        ParkingSpaceNumber mParkingSpace;
        bool mParked = false;
    };
}


#endif // microreactor_Parkable
