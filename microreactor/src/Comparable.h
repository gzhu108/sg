#pragma once
#ifndef sg_microreactor_Comparable
#define sg_microreactor_Comparable


namespace sg { namespace microreactor
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
}}


#endif // sg_microreactor_Comparable
