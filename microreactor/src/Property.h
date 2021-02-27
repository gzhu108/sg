#pragma once
#ifndef microreactor_Property
#define microreactor_Property

#pragma warning(disable : 4512)

#include <functional>
#include "Emittable.h"
#include "ScopeLock.h"
#include "Comparable.h"

#define PROPERTY(_name, _type, ...) \
    mutable microreactor::Property<_type> _name { std::string(#_name), ##__VA_ARGS__ }

#define PROPERTY_GET(_name, _type, _default, _member_getter) \
    PROPERTY(_name, _type, _default, \
        std::bind(&_member_getter, this, std::placeholders::_1), \
        nullptr)

#define PROPERTY_SET(_name, _type, _default, _member_setter) \
    PROPERTY(_name, _type, _default, \
        nullptr, \
        std::bind(&_member_setter, this, std::placeholders::_1, std::placeholders::_2))

#define PROPERTY_GET_SET(_name, _type, _default, _member_getter, _member_setter) \
    PROPERTY(_name, _type, _default, \
        std::bind(&_member_getter, this, std::placeholders::_1), \
        std::bind(&_member_setter, this, std::placeholders::_1, std::placeholders::_2))

#define PROPERTY_TYPE(_property) \
    decltype(_property)::ValueType


namespace microreactor
{
    template<class T>
    class Property
    {
    public:
        typedef T ValueType;
        typedef std::function<T (const ValueType&)> GetterType;
        typedef std::function<void (ValueType&, const T&)> SetterType;

        Signal<T>& ValueChanged = mValueChanged;

        explicit Property(const std::string& name)
            : mName(name)
        {
        }

        explicit Property(std::string&& name)
        {
            mName = std::move(name);
        }

        Property(const std::string& name, const T& defaultValue)
            : mName(name)
            , mValue(defaultValue)
        {
        }

        Property(std::string&& name, T&& defaultValue)
        {
            mName = std::move(name);
            mValue = std::move(defaultValue);
        }

        Property(const std::string& name, const GetterType& getter, const SetterType& setter)
            : mGetter(getter)
            , mSetter(setter)
            , mName(name)
        {
        }

        Property(std::string&& name, GetterType&& getter, SetterType&& setter)
        {
            mGetter = std::move(getter);
            mSetter = std::move(setter);
            mName = std::move(name);
        }

        Property(const std::string& name, const T& defaultValue, const GetterType& getter, const SetterType& setter)
            : mGetter(getter)
            , mSetter(setter)
            , mName(name)
            , mValue(defaultValue)
        {
        }

        Property(std::string&& name, T&& defaultValue, GetterType&& getter, SetterType&& setter)
        {
            mGetter = std::move(getter);
            mSetter = std::move(setter);
            mName = std::move(name);
            mValue = std::move(defaultValue);
        }

    public:
        std::string& Name() const { return mName; }
        const std::string& Name() { return mName; }

        T& ref()
        {
            ScopeLock<decltype(mLock)> scopeLock(mLock);
            if (mGetter == nullptr)
            {
                return mValue;
            }

            mValue = mGetter(mValue);
            return mValue;
        }

        T* operator ->()
        {
            return &ref();
        }

        const T& cref()
        {
            return ref();
        }

        T get() const
        {
            ScopeLock<decltype(mLock)> scopeLock(mLock);
            if (mGetter == nullptr)
            {
                return mValue;
            }

            return mGetter(mValue);
        }

        auto& set(const T& value)
        {
            ScopeLock<decltype(mLock)> scopeLock(mLock);
            if (Comparable<T>(mValue) != value)
            {
                if (mSetter == nullptr)
                {
                    mValue = value;
                }
                else
                {
                    mSetter(mValue, value);
                }
                
                // Signal property value changed
                mValueChanged(mValue);
            }

            return *this;
        }

    private:
        mutable std::recursive_mutex mLock;
        GetterType mGetter;
        SetterType mSetter;
        std::string mName;
        ValueType mValue {};
        Emittable<T> mValueChanged;
    };
}


#endif // microreactor_Property
