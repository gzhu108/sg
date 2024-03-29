#pragma once
#ifndef microreactor_BinarySerializer
#define microreactor_BinarySerializer

#include "Serializer.h"


namespace microreactor
{
    class BinarySerializer : public Serializer
    {
    public:
        BinarySerializer();
        virtual ~BinarySerializer();
        
    public:
        virtual bool Read(std::istream& stream, std::ostream& val) override;
        virtual bool Read(std::istream& stream, bool& val) override;
        virtual bool Read(std::istream& stream, char& val) override;
        virtual bool Read(std::istream& stream, uint8_t& val) override;
        virtual bool Read(std::istream& stream, int16_t& val) override;
        virtual bool Read(std::istream& stream, uint16_t& val) override;
        virtual bool Read(std::istream& stream, int32_t& val) override;
        virtual bool Read(std::istream& stream, uint32_t& val) override;
        virtual bool Read(std::istream& stream, int64_t& val) override;
        virtual bool Read(std::istream& stream, uint64_t& val) override;
        virtual bool Read(std::istream& stream, float& val) override;
        virtual bool Read(std::istream& stream, double& val) override;
        virtual bool Read(std::istream& stream, std::string& val) override;
        virtual bool Read(std::istream& stream, std::chrono::milliseconds& val) override;
        virtual bool Read(std::istream& stream, Serializable& val) override;

        virtual bool Write(std::istream& val, std::ostream& stream) override;
        virtual bool Write(bool val, std::ostream& stream) override;
        virtual bool Write(char val, std::ostream& stream) override;
        virtual bool Write(uint8_t val, std::ostream& stream) override;
        virtual bool Write(int16_t val, std::ostream& stream) override;
        virtual bool Write(uint16_t val, std::ostream& stream) override;
        virtual bool Write(int32_t val, std::ostream& stream) override;
        virtual bool Write(uint32_t val, std::ostream& stream) override;
        virtual bool Write(int64_t val, std::ostream& stream) override;
        virtual bool Write(uint64_t val, std::ostream& stream) override;
        virtual bool Write(float val, std::ostream& stream) override;
        virtual bool Write(double val, std::ostream& stream) override;
        virtual bool Write(const std::string& val, std::ostream& stream) override;
        virtual bool Write(const std::chrono::milliseconds& val, std::ostream& stream) override;
        virtual bool Write(const Serializable& val, std::ostream& stream) override;
    };
}


#endif // microreactor_BinarySerializer
