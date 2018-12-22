#pragma once
#ifndef sg_microreactor_Emittable
#define sg_microreactor_Emittable

#pragma warning(push)
#pragma warning(disable : 4512)

#include <algorithm>
#include <type_traits>
#include <list>
#include <map>
#include <functional>
#include "Exception.h"


namespace sg { namespace microreactor
{
    // Signal<SignalType>& SignalName = mEmittableMember;
    // typedef std::remove_reference<decltype(SignalName)>::type::SignalId SignalNameId
    template<typename T>
    class Signal
    {
    public:
        typedef std::function<void (const T&)> SignalType;
        typedef std::list<SignalType> SignalStore;  // Use std::list, because it's iterator won't move.
        typedef typename SignalStore::const_iterator SignalId;
        typedef std::map<uintptr_t, SignalId> SignalIdMap;

        SignalId Connect(SignalType signal)
        {
            mSignals.emplace_back(std::move(signal));
            return --mSignals.end();
        }

        void Connect(SignalType signal, const uintptr_t& key)
        {
            SignalId signalId = Connect(signal);
            mSignalIdMap[key] = std::move(signalId);
        }
        
        void Disconnect(SignalId& id)
        {
            mSignals.erase(id);
        }

        void Disconnect(const uintptr_t& key)
        {
            auto itr = mSignalIdMap.find(key);
            if (itr != mSignalIdMap.end())
            {
                Disconnect(itr->second);
                mSignalIdMap.erase(itr);
            }
        }

    protected:
        SignalStore mSignals;
        SignalIdMap mSignalIdMap;
    };
    
    template<>
    class Signal<void>
    {
    public:
        typedef std::function<void ()> SignalType;
        typedef std::list<SignalType> SignalStore;  // Use std::list, because it's iterator won't move.
        typedef SignalStore::const_iterator SignalId;
        typedef std::map<uintptr_t, SignalId> SignalIdMap;

        SignalId Connect(SignalType signal)
        {
            mSignals.emplace_back(std::move(signal));
            return --mSignals.end();
        }

        void Connect(SignalType signal, const uintptr_t& key)
        {
            SignalId signalId = Connect(signal);
            mSignalIdMap[key] = std::move(signalId);
        }
        
        void Disconnect(SignalId& id)
        {
            mSignals.erase(id);
        }

        void Disconnect(const uintptr_t& key)
        {
            auto itr = mSignalIdMap.find(key);
            if (itr != mSignalIdMap.end())
            {
                Disconnect(itr->second);
                mSignalIdMap.erase(itr);
            }
        }

    protected:
        SignalStore mSignals;
        SignalIdMap mSignalIdMap;
    };

    template<typename T>
    class Emittable : public Signal<T>
    {
    public:
        void operator ()(const T& value) const
        {
            // In case mSignals is changed during signaling, make a copy of the signal list.
            auto signalsCopy = Signal<T>::mSignals;
            for (const auto& signal : signalsCopy)
            {
                try
                {
                    signal(value);
                }
                catch (...)
                {
                    THROW(SignalException);
                }
            }
        }
    };
    
    template<>
    class Emittable<void> : public Signal<void>
    {
    public:
        void operator ()() const
        {
            // In case mSignals is changed during signaling, make a copy of the signal list.
            auto signalsCopy = Signal<void>::mSignals;
            for (const auto& signal : signalsCopy)
            {
                try
                {
                    signal();
                }
                catch (...)
                {
                    THROW(SignalException);
                }
            }
        }
    };
}}


#pragma warning(pop)
#endif // sg_microreactor_Emittable
