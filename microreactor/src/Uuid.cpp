#include "Uuid.h"

using namespace microreactor;


Uuid::Uuid()
{
    Clear();
}

Uuid::Uuid(const Uuid& uuid)
{
#ifdef _MSC_VER
    mUuid = uuid.mUuid;
#else
    uuid_copy(mUuid, uuid.mUuid);
#endif
}

Uuid::Uuid(const std::string& uuid)
{
#ifdef _MSC_VER
    unsigned char uuidString[64] = { 0 };
    uuid.copy(reinterpret_cast<char*>(uuidString), uuid.length() < sizeof(uuidString) ? uuid.length() : sizeof(uuidString) - 1);
    UuidFromStringA(uuidString, &mUuid);
#else
    uuid_parse(uuid.c_str(), mUuid);
#endif
}

Uuid::~Uuid()
{
}

bool Uuid::Serialize(Serializer& serializer, std::ostream& stream) const
{
    return SerializerWrite(serializer, (char*)&mUuid, (char*)&mUuid + 16, stream);
}

bool Uuid::Deserialize(std::istream& stream, Serializer& serializer)
{
    return SerializerRead(serializer, stream, (char*)&mUuid, 16);
}

Uuid& Uuid::operator =(const Uuid& uuid)
{
#ifdef _MSC_VER
    mUuid = uuid.mUuid;
#else
    uuid_copy(mUuid, uuid.mUuid);
#endif

    return *this;
}

bool Uuid::operator <(const Uuid& uuid) const
{
#ifdef _MSC_VER
    RPC_STATUS status = RPC_S_OK;
    return UuidCompare(const_cast<UUID*>(&mUuid), const_cast<UUID*>(&uuid.mUuid), &status) < 0;
#else
    return uuid_compare(mUuid, uuid.mUuid) < 0;
#endif
}

std::string Uuid::ToString() const
{
    std::string uuid;

#ifdef _MSC_VER
    RPC_CSTR uuidString = nullptr;
    UuidToString(&mUuid, &uuidString);
    if (uuidString != nullptr)
    {
        uuid = reinterpret_cast<char*>(uuidString);
        RpcStringFree(&uuidString);
    }
#else
    char uuidString[64] = { 0 };
    uuid_unparse(mUuid, uuidString);
    uuid = uuidString;
#endif

    return uuid;
}

bool Uuid::IsNull() const
{
#ifdef _MSC_VER
    RPC_STATUS status = RPC_S_OK;
    return UuidIsNil(const_cast<UUID*>(&mUuid), &status) > 0;
#else
    return uuid_is_null(mUuid) > 0;
#endif
}

void Uuid::Clear()
{
#ifdef _MSC_VER
    UuidCreateNil(&mUuid);
#else
    uuid_clear(mUuid);
#endif
}

Uuid Uuid::GenerateUuid()
{
    Uuid uuid;

#ifdef _MSC_VER
    UuidCreate(&uuid.mUuid);
#else
    int retrySafe = 0;
    while (uuid_generate_time_safe(uuid.mUuid) != 0)
    {
        if (retrySafe <= 0)
        {
            uuid_generate(uuid.mUuid);
            break;
        }
        
        retrySafe--;
    }
#endif

    return uuid;
}
