#include "BinarySerializer.h"
#include "Endianness.h"

using namespace microreactor;


BinarySerializer::BinarySerializer()
{
}

BinarySerializer::~BinarySerializer()
{
}

bool BinarySerializer::Read(std::istream& stream, std::ostream& val)
{
    return Serializer::Read(stream, val);
}

bool BinarySerializer::Read(std::istream& stream, bool& val)
{
    char buffer = 0;
    Read(stream, buffer);
    val = buffer != 0;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Read(std::istream& stream, char& val)
{
    stream.read(reinterpret_cast<char*>(&val), sizeof(val));
    val = Endian::HostOrder(val);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Read(std::istream& stream, uint8_t& val)
{
    stream.read(reinterpret_cast<char*>(&val), sizeof(val));
    val = Endian::HostOrder(val);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Read(std::istream& stream, int16_t& val)
{
    stream.read(reinterpret_cast<char*>(&val), sizeof(val));
    val = Endian::HostOrder(val);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Read(std::istream& stream, uint16_t& val)
{
    stream.read(reinterpret_cast<char*>(&val), sizeof(val));
    val = Endian::HostOrder(val);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Read(std::istream& stream, int32_t& val)
{
    stream.read(reinterpret_cast<char*>(&val), sizeof(val));
    val = Endian::HostOrder(val);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Read(std::istream& stream, uint32_t& val)
{
    stream.read(reinterpret_cast<char*>(&val), sizeof(val));
    val = Endian::HostOrder(val);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Read(std::istream& stream, int64_t& val)
{
    stream.read(reinterpret_cast<char*>(&val), sizeof(val));
    val = Endian::HostOrder(val);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Read(std::istream& stream, uint64_t& val)
{
    stream.read(reinterpret_cast<char*>(&val), sizeof(val));
    val = Endian::HostOrder(val);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Read(std::istream& stream, float& val)
{
    stream.read(reinterpret_cast<char*>(&val), sizeof(val));
    val = Endian::HostOrder(val);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Read(std::istream& stream, double& val)
{
    stream.read(reinterpret_cast<char*>(&val), sizeof(val));
    val = Endian::HostOrder(val);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Read(std::istream& stream, std::string& val)
{
    return SerializerRead(*this, stream, val);
}

bool BinarySerializer::Read(std::istream& stream, Serializable& val)
{
    return Serializer::Read(stream, val);
}

bool BinarySerializer::Write(std::istream& val, std::ostream& stream)
{
    return Serializer::Write(val, stream);
}

bool BinarySerializer::Write(bool val, std::ostream& stream)
{
    char buffer = (char)val;
    Write(buffer, stream);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Write(char val, std::ostream& stream)
{
    val = Endian::NetOrder(val);
    stream.write(reinterpret_cast<char*>(&val), sizeof(val));
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Write(uint8_t val, std::ostream& stream)
{
    val = Endian::NetOrder(val);
    stream.write(reinterpret_cast<char*>(&val), sizeof(val));
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Write(int16_t val, std::ostream& stream)
{
    val = Endian::NetOrder(val);
    stream.write(reinterpret_cast<char*>(&val), sizeof(val));
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Write(uint16_t val, std::ostream& stream)
{
    val = Endian::NetOrder(val);
    stream.write(reinterpret_cast<char*>(&val), sizeof(val));
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Write(int32_t val, std::ostream& stream)
{
    val = Endian::NetOrder(val);
    stream.write(reinterpret_cast<char*>(&val), sizeof(val));
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Write(uint32_t val, std::ostream& stream)
{
    val = Endian::NetOrder(val);
    stream.write(reinterpret_cast<char*>(&val), sizeof(val));
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Write(int64_t val, std::ostream& stream)
{
    val = Endian::NetOrder(val);
    stream.write(reinterpret_cast<char*>(&val), sizeof(val));
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Write(uint64_t val, std::ostream& stream)
{
    val = Endian::NetOrder(val);
    stream.write(reinterpret_cast<char*>(&val), sizeof(val));
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Write(float val, std::ostream& stream)
{
    val = Endian::NetOrder(val);
    stream.write(reinterpret_cast<char*>(&val), sizeof(val));
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Write(double val, std::ostream& stream)
{
    val = Endian::NetOrder(val);
    stream.write(reinterpret_cast<char*>(&val), sizeof(val));
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool BinarySerializer::Write(const std::string& val, std::ostream& stream)
{
    return SerializerWrite(*this, val, stream);
}

bool BinarySerializer::Write(const Serializable& val, std::ostream& stream)
{
    return Serializer::Write(val, stream);
}
