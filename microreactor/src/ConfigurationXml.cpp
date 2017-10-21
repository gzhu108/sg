#include <rapidxml.hpp>
#include "ConfigurationXml.h"
#include <fstream>
#include <functional>
#include <stdlib.h>
#include <cstdlib>

using namespace sg::microreactor;


ConfigurationXml::ConfigurationXml(const std::string& configFilePath, const std::string& rootNode)
    : mConfigFilePath(configFilePath)
    , mRootNode(rootNode)
    , mXmlDoc(new rapidxml::xml_document<char>())
{
    Refresh();
}

ConfigurationXml::~ConfigurationXml()
{
}

void ConfigurationXml::Refresh()
{
    std::ifstream xmlFile;
    xmlFile.open(mConfigFilePath, std::ios::binary);
    if (!xmlFile.is_open())
    {
        LOG("Cannot open configuration file: %s", mConfigFilePath.c_str());
        return;
    }
    
    mConfigDoc.assign(std::istreambuf_iterator<char>(xmlFile), std::istreambuf_iterator<char>());
    xmlFile.close();
    LOG("\n\n%s\n", mConfigDoc.c_str());

    if (mConfigDoc.empty())
    {
        if (mFileHash)
        {
            mValueUpdated();
            return;
        }
        return;
    }

    std::hash<std::string> stringHash;
    uint64_t newHash = stringHash(mConfigDoc);
    
    mConfigDoc.push_back('\0');
    mXmlDoc->parse<0>(&mConfigDoc[0]);
    
    if (newHash != mFileHash)
    {
        mFileHash = newHash;
        mValueUpdated();
    }
}

std::string& ConfigurationXml::GetValue(const std::string& name, std::string& value)
{
    if (!mConfigDoc.empty())
    {
        auto root = mXmlDoc->first_node(mRootNode.c_str());
        if (root)
        {
            auto node = root->first_node(name.c_str());
            if (node)
            {
                value = node->value();
                return value;
            }
        }
    }
    
    return value;
}

uint8_t& ConfigurationXml::GetValue(const std::string& name, uint8_t& value)
{
    if (!mConfigDoc.empty())
    {
        auto root = mXmlDoc->first_node(mRootNode.c_str());
        if (root)
        {
            auto node = root->first_node(name.c_str());
            if (node)
            {
                value = (uint8_t)std::atoi(node->value());
                return value;
            }
        }
    }
    
    return value;
}

char& ConfigurationXml::GetValue(const std::string& name, char& value)
{
    if (!mConfigDoc.empty())
    {
        auto root = mXmlDoc->first_node(mRootNode.c_str());
        if (root)
        {
            auto node = root->first_node(name.c_str());
            if (node)
            {
                value = (char)std::atoi(node->value());
                return value;
            }
        }
    }
    
    return value;
}

uint16_t& ConfigurationXml::GetValue(const std::string& name, uint16_t& value)
{
    if (!mConfigDoc.empty())
    {
        auto root = mXmlDoc->first_node(mRootNode.c_str());
        if (root)
        {
            auto node = root->first_node(name.c_str());
            if (node)
            {
                value = (uint16_t)std::atoi(node->value());
                return value;
            }
        }
    }
    
    return value;
}

int16_t& ConfigurationXml::GetValue(const std::string& name, int16_t& value)
{
    if (!mConfigDoc.empty())
    {
        auto root = mXmlDoc->first_node(mRootNode.c_str());
        if (root)
        {
            auto node = root->first_node(name.c_str());
            if (node)
            {
                value = (int16_t)std::atoi(node->value());
                return value;
            }
        }
    }
    
    return value;
}

uint32_t& ConfigurationXml::GetValue(const std::string& name, uint32_t& value)
{
    if (!mConfigDoc.empty())
    {
        auto root = mXmlDoc->first_node(mRootNode.c_str());
        if (root)
        {
            auto node = root->first_node(name.c_str());
            if (node)
            {
                value = (uint32_t)std::atoi(node->value());
                return value;
            }
        }
    }
    
    return value;
}

int32_t& ConfigurationXml::GetValue(const std::string& name, int32_t& value)
{
    if (!mConfigDoc.empty())
    {
        auto root = mXmlDoc->first_node(mRootNode.c_str());
        if (root)
        {
            auto node = root->first_node(name.c_str());
            if (node)
            {
                value = std::atoi(node->value());
                return value;
            }
        }
    }
    
    return value;
}

uint64_t& ConfigurationXml::GetValue(const std::string& name, uint64_t& value)
{
    if (!mConfigDoc.empty())
    {
        auto root = mXmlDoc->first_node(mRootNode.c_str());
        if (root)
        {
            auto node = root->first_node(name.c_str());
            if (node)
            {
                value = std::strtoul(node->value(), nullptr, 0);
                return value;
            }
        }
    }
    
    return value;
}

int64_t& ConfigurationXml::GetValue(const std::string& name, int64_t& value)
{
    if (!mConfigDoc.empty())
    {
        auto root = mXmlDoc->first_node(mRootNode.c_str());
        if (root)
        {
            auto node = root->first_node(name.c_str());
            if (node)
            {
                value = std::strtoul(node->value(), nullptr, 0);
                return value;
            }
        }
    }
    
    return value;
}

float& ConfigurationXml::GetValue(const std::string& name, float& value)
{
    if (!mConfigDoc.empty())
    {
        auto root = mXmlDoc->first_node(mRootNode.c_str());
        if (root)
        {
            auto node = root->first_node(name.c_str());
            if (node)
            {
                value = (float)std::atof(node->value());
                return value;
            }
        }
    }
    
    return value;
}

double& ConfigurationXml::GetValue(const std::string& name, double& value)
{
    if (!mConfigDoc.empty())
    {
        auto root = mXmlDoc->first_node(mRootNode.c_str());
        if (root)
        {
            auto node = root->first_node(name.c_str());
            if (node)
            {
                value = strtod(node->value(), nullptr);
                return value;
            }
        }
    }
    
    return value;
}

bool& ConfigurationXml::GetValue(const std::string& name, bool& value)
{
    if (!mConfigDoc.empty())
    {
        auto root = mXmlDoc->first_node(mRootNode.c_str());
        if (root)
        {
            auto node = root->first_node(name.c_str());
            if (node)
            {
                value = std::string("true").compare(node->value()) == 0;
                return value;
            }
        }
    }
    
    return value;
}
