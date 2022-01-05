#include "Serializer.h"
#include "Serializable.h"

using namespace microreactor;

    
Serializer::Serializer()
{
}

Serializer::~Serializer()
{
}

bool Serializer::Read(std::istream& stream, std::ostream& val)
{
    stream >> val.rdbuf();
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Read(std::istream& stream, bool& val)
{
    stream >> val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Read(std::istream& stream, char& val)
{
    std::operator >>(stream, val);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Read(std::istream& stream, uint8_t& val)
{
    std::operator >>(stream, val);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Read(std::istream& stream, int16_t& val)
{
    stream >> val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Read(std::istream& stream, uint16_t& val)
{
    stream >> val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Read(std::istream& stream, int32_t& val)
{
    stream >> val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Read(std::istream& stream, uint32_t& val)
{
    stream >> val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Read(std::istream& stream, int64_t& val)
{
    stream >> val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Read(std::istream& stream, uint64_t& val)
{
    stream >> val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Read(std::istream& stream, float& val)
{
    stream >> val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Read(std::istream& stream, double& val)
{
    stream >> val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Read(std::istream& stream, char* val, uint64_t size)
{
    stream.read((char*)val, size);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Read(std::istream& stream, std::string& val)
{
    // Use the default initializer {}, equivalent to std::istreambuf_iterator<char>()
    val.assign(std::istreambuf_iterator<char>(stream), {});
    stream.clear();
    stream.ignore(std::numeric_limits<std::streamsize>::max());
    return !stream.fail() && !stream.bad();
}

bool Serializer::Read(std::istream& stream, std::chrono::milliseconds& val)
{
    int64_t intVal = 0;
    bool result = Read(stream, intVal);
    val = std::chrono::milliseconds(intVal);
    return result;
}

bool Serializer::Read(std::istream& stream, Serializable& val)
{
    val.Deserialize(stream, *this);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(std::istream& val, std::ostream& stream)
{
    stream << val.rdbuf();
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(bool val, std::ostream& stream)
{
    stream << val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(char val, std::ostream& stream)
{
    stream << val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(uint8_t val, std::ostream& stream)
{
    stream << val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(int16_t val, std::ostream& stream)
{
    stream << val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(uint16_t val, std::ostream& stream)
{
    stream << val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(int32_t val, std::ostream& stream)
{
    stream << val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(uint32_t val, std::ostream& stream)
{
    stream << val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(int64_t val, std::ostream& stream)
{
    stream << val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(uint64_t val, std::ostream& stream)
{
    stream << val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(float val, std::ostream& stream)
{
    stream << val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(double val, std::ostream& stream)
{
    stream << val;
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(const char* val, uint64_t size, std::ostream& stream)
{
    stream.write((char*)val, size);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(const char* val, std::ostream& stream)
{
    stream.write((char*)val, strlen(val));
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(const std::string& val, std::ostream& stream)
{
    stream.write(&val[0], val.length());
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(const std::chrono::milliseconds& val, std::ostream& stream)
{
    stream << val.count();
    return !stream.eof() && !stream.fail() && !stream.bad();
}

bool Serializer::Write(const Serializable& val, std::ostream& stream)
{
    val.Serialize(*this, stream);
    return !stream.eof() && !stream.fail() && !stream.bad();
}

