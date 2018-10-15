#pragma once
#ifndef worldapi_World
#define worldapi_World

#include <vector>
#include "Parkable.h"
#include "WorldId.h"
#include "Point.h"
#include "Rectangle.h"


namespace worldapi
{
    struct World : sg::microreactor::Serializable
    {
        WorldId mId;
        Rectangle<float> mWorldBoundary;
        std::vector<Point<float>> mItems;

        World()
            : mId(DefaultWorldId)
            , mWorldBoundary(0.0, 0.0, 0.0, 0.0)
        {
        }

        World(const WorldId& id, const Rectangle<float>& worldBoundary, const std::vector<Point<float>>& items)
            : mId(id)
            , mWorldBoundary(worldBoundary)
            , mItems(items)
        {
        }

        World(const World& other)
            : World(other.mId, other.mWorldBoundary, other.mItems)
        {
        }

        World& operator =(const World& other)
        {
            mId = other.mId;
            mWorldBoundary = other.mWorldBoundary;
            mItems = other.mItems;
            return *this;
        }

        bool operator <(const World& other) const
        {
            return mId < other.mId;
        }

        virtual bool Serialize(sg::microreactor::Serializer& serializer, std::ostream& stream) const override
        {
            return serializer.Write(mId, stream)
                && serializer.Write(mWorldBoundary, stream)
                && SerializerWrite(serializer, mItems, stream);
        }

        virtual bool Deserialize(std::istream& stream, sg::microreactor::Serializer& serializer) override
        {
            if (!serializer.Read(stream, mId))
            {
                return false;
            }

            if (!serializer.Read(stream, mWorldBoundary))
            {
                return false;
            }

            if (!SerializerRead(serializer, stream, mItems))
            {
                return false;
            }

            return true;
        }
    };
}

#endif // worldapi_World
