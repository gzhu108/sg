#pragma once
#ifndef sg_microreactor_Parkable
#define sg_microreactor_Parkable

#include "Common.h"
#include "Shareable.h"


namespace sg { namespace microreactor
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
}}


#endif // sg_microreactor_Parkable
