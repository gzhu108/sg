#pragma once
#ifndef microreactor_Comparable
#define microreactor_Comparable


namespace microreactor
{
    template<class T, class Compare = std::less<T>>
    class Comparable
    {
    public:
        explicit Comparable(const T& object)
            : mObject(object)
        {
        }
        
        bool operator <(const T& other) const
        {
            return mLess(mObject, other);
        }
        
        bool operator >(const T& other) const
        {
            return mLess(other, mObject);
        }
        
        bool operator ==(const T& other) const
        {
            return !mLess(mObject, other) && !mLess(other, mObject);
        }
        
        bool operator <=(const T& other) const
        {
            return mLess(mObject, other) || !mLess(other, mObject);
        }
        
        bool operator >=(const T& other) const
        {
            return mLess(other, mObject) || !mLess(mObject, other);
        }

        bool operator !=(const T& other) const
        {
            return mLess(mObject, other) || mLess(other, mObject);
        }
        
    protected:
        const T& mObject;
        Compare mLess;
    };
}


#endif // microreactor_Comparable
