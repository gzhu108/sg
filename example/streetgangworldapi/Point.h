#pragma once
#ifndef streetgangworldapi_Point
#define streetgangworldapi_Point

#include "Serializable.h"


namespace streetgangworldapi
{
    template<typename T>
    struct Point : sg::microreactor::Serializable
    {
        T mX;
        T mY;
        T mZ;

        Point()
        {
        }

        Point(const T& x, const T& y, const T& z)
        {
            mX = x;
            mY = y;
            mZ = z;
        }

        bool operator <(const Point& other) const
        {
            if (mX < other.mX)
            {
                return true;
            }
            else if (mX > other.mX)
            {
                return false;
            }
            else if (mY < other.mY)
            {
                return true;
            }
            else if (mY > other.mY)
            {
                return false;
            }
            else if (mZ < other.mZ)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        virtual bool Serialize(sg::microreactor::Serializer& serializer, std::ostream& stream) const override
        {
            return serializer.Write(mX, stream) && serializer.Write(mY, stream) && serializer.Write(mZ, stream);
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

            if (!serializer.Read(stream, mZ))
            {
                return false;
            }
            
            return true;
        }
    };
}

#endif // streetgangworldapi_Point
