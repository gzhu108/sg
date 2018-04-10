#pragma once
#ifndef sg_microreactor_Park
#define sg_microreactor_Park

#include <unordered_map>
#include "Parkable.h"


namespace sg { namespace microreactor
{
    class Park
    {
    public:
        virtual ~Park();

    public:
        static Park& ParkingLot();

        virtual bool Add(uintptr_t parking, std::shared_ptr<Parkable> parkable);
        virtual uint64_t Remove(uintptr_t parking, std::vector<std::shared_ptr<Parkable>>& parkables);
        virtual uint64_t Get(uintptr_t parking, std::vector<std::shared_ptr<Parkable>>& parkables);

    protected:
        static Park mParkingLot;
        mutable std::recursive_mutex mLock;
        std::unordered_multimap<uintptr_t, std::shared_ptr<Parkable>> mParkables;

    private:
        Park();
    };
}}


#endif // sg_microreactor_Park
