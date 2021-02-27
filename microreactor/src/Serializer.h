#pragma once
#ifndef microreactor_Serializer
#define microreactor_Serializer

#pragma warning(disable : 4512)

#include "Common.h"


namespace microreactor
{
    class Serializable;
    
    template<class SerializerType, class ArrayType>
    bool SerializerRead(SerializerType& serializer, std::istream& stream, ArrayType array, uint64_t size)
    {
        uint16_t count = 0;
        bool result = serializer.ReadCount(stream, count);
        if (!result || size < count)
        {
            return false;
        }
        
        for (int16_t i = 0; i < count && result; i++)
        {
            result = serializer.Read(stream, array[i]);
            if (!result)
            {
                return false;
            }
        }
        
        return result;
    }
    
    template<class SerializerType, class ContainerType>
    bool SerializerRead(SerializerType& serializer, std::istream& stream, ContainerType& container)
    {
        uint16_t count = 0;
        bool result = serializer.ReadCount(stream, count);
        if (!result)
        {
            return false;
        }

        container.resize(count);
        auto itr = container.begin();
        while (itr != container.end() && result)
        {
            result = serializer.Read(stream, *itr);
            if (!result)
            {
                return false;
            }

            itr++;
        }
        
        return result;
    }
        
    template<class SerializerType, class InputIt>
    bool SerializerWrite(SerializerType& serializer, InputIt first, InputIt last, std::ostream& stream)
    {
        uint16_t count = (uint16_t)std::distance(first, last);
        if (count < 0)
        {
            return false;
        }
        
        bool result = serializer.WriteCount(count, stream);
        while(first != last && result)
        {
            result = serializer.Write(*first, stream);
            if (!result)
            {
                return false;
            }

            first++;
        }
        
        return result;
    }
    
    template<class SerializerType, class ContainerType>
    bool SerializerWrite(SerializerType& serializer, const ContainerType& container, std::ostream& stream)
    {
        return SerializerWrite(serializer, container.begin(), container.end(), stream);
    }

    template<typename StreamType>
    uint64_t GetStreamSize(StreamType& stream)
    {
        if (stream.eof() || stream.fail() || stream.bad())
        {
            return 0;
        }

        auto current = stream.tellg();
        stream.seekg(0, stream.end);
        uint64_t size = stream.tellg() - current;
        stream.seekg(current, stream.beg);
        stream.clear();
        return size;
    }
    
    class Serializer
    {
    public:
        Serializer();
        virtual ~Serializer();
        
    public:
        virtual bool Read(std::istream& stream, std::ostream& val);
        virtual bool Read(std::istream& stream, bool& val);
        virtual bool Read(std::istream& stream, char& val);
        virtual bool Read(std::istream& stream, uint8_t& val);
        virtual bool Read(std::istream& stream, int16_t& val);
        virtual bool Read(std::istream& stream, uint16_t& val);
        virtual bool Read(std::istream& stream, int32_t& val);
        virtual bool Read(std::istream& stream, uint32_t& val);
        virtual bool Read(std::istream& stream, int64_t& val);
        virtual bool Read(std::istream& stream, uint64_t& val);
        virtual bool Read(std::istream& stream, float& val);
        virtual bool Read(std::istream& stream, double& val);
        virtual bool Read(std::istream& stream, char* val, uint64_t size);
        virtual bool Read(std::istream& stream, std::string& val);
        virtual bool Read(std::istream& stream, Serializable& val);
        
        virtual bool ReadCount(std::istream& stream, uint16_t& count)
        {
            return Read(stream, count);
        }

        template <class ValueType>
        bool Read(std::istream& stream, std::shared_ptr<ValueType>& val)
        {
            val = std::make_shared<ValueType>();
            return Read(stream, *val);
        }

        template <typename T>
        bool ReadProperty(std::istream& stream, T& property)
        {
            typename T::ValueType value;
            if (Read(stream, value))
            {
                property.set(value);
                return true;
            }
            return false;
        }
        
        virtual bool Write(std::istream& val, std::ostream& stream);
        virtual bool Write(bool val, std::ostream& stream);
        virtual bool Write(char val, std::ostream& stream);
        virtual bool Write(uint8_t val, std::ostream& stream);
        virtual bool Write(int16_t val, std::ostream& stream);
        virtual bool Write(uint16_t val, std::ostream& stream);
        virtual bool Write(int32_t val, std::ostream& stream);
        virtual bool Write(uint32_t val, std::ostream& stream);
        virtual bool Write(int64_t val, std::ostream& stream);
        virtual bool Write(uint64_t val, std::ostream& stream);
        virtual bool Write(float val, std::ostream& stream);
        virtual bool Write(double val, std::ostream& stream);
        virtual bool Write(const char* val, uint64_t size, std::ostream& stream);
        virtual bool Write(const char* val, std::ostream& stream);
        virtual bool Write(const std::string& val, std::ostream& stream);
        virtual bool Write(const Serializable& val, std::ostream& stream);
        
        virtual bool WriteCount(uint16_t count, std::ostream& stream)
        {
            return Write(count, stream);
        }

        template <class ValueType>
        bool Write(const std::shared_ptr<ValueType>& val, std::ostream& stream)
        {
            return Write(*val, stream);
        }
    };
}


#endif // microreactor_Serializer
