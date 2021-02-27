#pragma once
#ifndef microreactor_Uuid
#define microreactor_Uuid

#ifdef _MSC_VER
#include "Common.h"
#include <rpc.h>
#pragma comment(lib, "Rpcrt4.lib")
#else
#include <uuid/uuid.h>
#endif

#include "Serializable.h"


namespace microreactor
{
    class Uuid : public Serializable
    {
    public:
        Uuid();
        Uuid(const Uuid& uuid);
        explicit Uuid(const std::string& uuid);
        virtual ~Uuid();

    public:
        // Serializable
        virtual bool Serialize(Serializer& serializer, std::ostream& stream) const override;
        virtual bool Deserialize(std::istream& stream, Serializer& serializer) override;

        Uuid& operator =(const Uuid& uuid);
        bool operator <(const Uuid& uuid) const;

        std::string ToString() const;
        bool IsNull() const;
        void Clear();

        static Uuid GenerateUuid();

#ifdef _MSC_VER
    protected:
        UUID mUuid;
#else
    protected:
        // typedef unsigned char uuid_t[16]
        uuid_t mUuid;
#endif
    };
}

#endif // microreactor_Uuid
