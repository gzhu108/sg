#pragma once
#ifndef streetgangworldapi_Rectangle
#define streetgangworldapi_Rectangle

#include "Serializable.h"


namespace streetgangworldapi
{
    template<typename T>
    struct Rectangle : sg::microreactor::Serializable
    {
        T mX;
        T mY;
        T mW;
        T mH;

        Rectangle()
        {
        }

        Rectangle(const Rectangle& other)
        {
            mX = other.mX;
            mY = other.mY;
            mW = other.mW;
            mH = other.mH;
        }

        Rectangle(const T& x, const T& y, const T& w, const T& h)
        {
            mX = x;
            mY = y;
            mW = w;
            mH = h;
        }

        Rectangle& operator =(const Rectangle& other)
        {
            mX = other.mX;
            mY = other.mY;
            mW = other.mW;
            mH = other.mH;
            return *this;
        }

        bool operator <(const Rectangle& other) const
        {
            // Compare area
            return (mW * mH) < (other.mW * other.mH);
        }

        virtual bool Serialize(sg::microreactor::Serializer& serializer, std::ostream& stream) const override
        {
            return serializer.Write(mX, stream)
                && serializer.Write(mY, stream)
                && serializer.Write(mW, stream)
                && serializer.Write(mH, stream);
        }
        
        virtual bool Deserialize(std::istream& stream, sg::microreactor::Serializer& serializer) override
        {
            if (!serializer.Read(stream, mX))
            {
                return false;
            }

            if (!serializer.Read(stream, mY))
            {
                return false;
            }

            if (!serializer.Read(stream, mW))
            {
                return false;
            }

            if (!serializer.Read(stream, mH))
            {
                return false;
            }
            
            return true;
        }
    };
}

#endif // streetgangworldapi_Rectangle
