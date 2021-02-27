#pragma once
#ifndef metricatorapi_MetricatorLogger
#define metricatorapi_MetricatorLogger

#include "Connection.h"


namespace metricatorapi
{
    class MetricatorLogger
    {
    public:
        MetricatorLogger(const std::string& protocol, const std::string& address, uint16_t port);
        virtual ~MetricatorLogger();
        
    public:
        void Log(const std::string& text);

    protected:
        std::shared_ptr<microreactor::Connection> mConnection;
    };
}

#endif // metricatorapi_MetricatorLogger
