#include "QuicVariableLengthInteger.h"
#include "Endianness.h"

using namespace microreactor;


char* QuicVariableLengthInteger::EncodeMessage(char* first, char* last) const
{
    int8_t type = Type();
    if (!first || !last || last <= first || std::distance(first, last) <= type)
    {
        return last;
    }

    int64_t encodedInteger = Endian::NetOrder(mValue);
    char* mostSignificantByte = (char*)&encodedInteger;
    switch (type)
    {
    case IntegerType::Int6Bit:
        encodedInteger = Endian::NetOrder(((int8_t)(mValue & 0x000000000000003f)) & type);
        mostSignificantByte = (char*)&encodedInteger + 56;
        break;
    case IntegerType::Int14Bit:
        encodedInteger = Endian::NetOrder(((int16_t)(mValue & 0x0000000000003fff)) & type);
        mostSignificantByte = (char*)&encodedInteger + 48;
        break;
    case IntegerType::Int30Bit:
        encodedInteger = Endian::NetOrder(((int32_t)(mValue & 0x000000003fffffff)) & type);
        mostSignificantByte = (char*)&encodedInteger + 32;
        break;
    case IntegerType::Int62Bit:
    default:
        break;
    }

    *mostSignificantByte &= type << 6;
    char* leastSignificantByte = mostSignificantByte + type;
    for (char* temp = mostSignificantByte; temp <= leastSignificantByte; ++temp)
    {
        *first = *temp;
        ++first;
    }

    return first;
}

const char* QuicVariableLengthInteger::DecodeMessage(const char* first, const char* last)
{
    if (!first || !last || last <= first)
    {
        return last;
    }

    int8_t type = (*first) >> 6;
    if (std::distance(first, last) <= type)
    {
        return last;
    }

    mValue = 0;
    char* temp = (char*)&mValue;
    *temp = (*first) << 2 >> 2;
    ++temp;
    ++first;

    while (type--)
    {
        *temp++ = *first++;
    }

    mValue = Endian::HostOrder(mValue);

    return first;
}

QuicVariableLengthInteger::IntegerType QuicVariableLengthInteger::Type() const
{
    if (mValue & 0xffffffffc0000000)
    {
        return IntegerType::Int62Bit;
    }
    else if (mValue & 0xffffffffffffc000)
    {
        return IntegerType::Int30Bit;
    }
    else if (mValue & 0xffffffffffffffc0)
    {
        return IntegerType::Int14Bit;
    }
    else
    {
        return IntegerType::Int6Bit;
    }
}
