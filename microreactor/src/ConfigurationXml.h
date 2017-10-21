#pragma once
#ifndef sg_microreactor_ConfigurationXml
#define sg_microreactor_ConfigurationXml

#include "Configuration.h"


namespace rapidxml
{
    template<class Ch> class xml_document;
}

namespace sg { namespace microreactor
{
    class ConfigurationXml : public Configuration
    {
    public:
        ConfigurationXml(const std::string& configFile, const std::string& rootNode);
        virtual ~ConfigurationXml();

    public:
        virtual void Refresh() override;

        virtual std::string& GetValue(const std::string& name, std::string& value) override;
        virtual uint8_t& GetValue(const std::string& name, uint8_t& value) override;
        virtual char& GetValue(const std::string& name, char& value) override;
        virtual uint16_t& GetValue(const std::string& name, uint16_t& value) override;
        virtual int16_t& GetValue(const std::string& name, int16_t& value) override;
        virtual uint32_t& GetValue(const std::string& name, uint32_t& value) override;
        virtual int32_t& GetValue(const std::string& name, int32_t& value) override;
        virtual uint64_t& GetValue(const std::string& name, uint64_t& value) override;
        virtual int64_t& GetValue(const std::string& name, int64_t& value) override;
        virtual float& GetValue(const std::string& name, float& value) override;
        virtual double& GetValue(const std::string& name, double& value) override;
        virtual bool& GetValue(const std::string& name, bool& value) override;

    protected:
        std::string mConfigFilePath;
        std::string mConfigDoc;
        std::string mRootNode;
        uint64_t mFileHash{0};
        std::unique_ptr<rapidxml::xml_document<char>> mXmlDoc;
    };
}}


#endif // sg_microreactor_ConfigurationXml
