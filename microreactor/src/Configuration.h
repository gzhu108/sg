#pragma once
#ifndef microreactor_Configuration
#define microreactor_Configuration

#pragma warning(push)
#pragma warning(disable : 4512)

#include "Common.h"


namespace microreactor
{
    class Configuration
    {
    public:
        Signal<void>& ValueUpdated = mValueUpdated;

    public:
        virtual void Refresh() = 0;

        virtual std::string& GetValue(const std::string& name, std::string& value) = 0;
        virtual uint8_t& GetValue(const std::string& name, uint8_t& value) = 0;
        virtual char& GetValue(const std::string& name, char& value) = 0;
        virtual uint16_t& GetValue(const std::string& name, uint16_t& value) = 0;
        virtual int16_t& GetValue(const std::string& name, int16_t& value) = 0;
        virtual uint32_t& GetValue(const std::string& name, uint32_t& value) = 0;
        virtual int32_t& GetValue(const std::string& name, int32_t& value) = 0;
        virtual uint64_t& GetValue(const std::string& name, uint64_t& value) = 0;
        virtual int64_t& GetValue(const std::string& name, int64_t& value) = 0;
        virtual float& GetValue(const std::string& name, float& value) = 0;
        virtual double& GetValue(const std::string& name, double& value) = 0;
        virtual bool& GetValue(const std::string& name, bool& value) = 0;

    protected:
        Emittable<void> mValueUpdated;
    };
}


#pragma warning(pop)
#endif // microreactor_Configuration
