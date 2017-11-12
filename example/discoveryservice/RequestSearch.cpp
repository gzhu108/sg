#include "RequestSearch.h"
#include "StringUtility.h"

using namespace sg::microreactor;
using namespace sg::service;


static const std::string M_SEARCH("M-SEARCH * HTTP/1.1");
static const std::string HOST("HOST:");
static const std::string MAN("MAN: ssdp:discover");
static const std::string MX("MX:");
static const std::string ST("ST:");
static const std::string USER_AGENT("USER-AGENT:");


RequestSearch::RequestSearch()
{
    //ExpectResponse.set(false);
}

RequestSearch::~RequestSearch()
{
}

bool RequestSearch::Serialize(Serializer& serializer, std::ostream& stream) const
{
    serializer.Write(M_SEARCH + "\r\n", stream);
    serializer.Write(HOST + " " + Endpoint.get() + "\r\n", stream);
    serializer.Write(MAN + "\r\n", stream);

    if (Mx.get())
    {
        serializer.Write(MX + " " + std::to_string(Mx.get()) + "\r\n", stream);
    }

    serializer.Write(ST + " " + St.get() + "\r\n", stream);

    if (!UserAgent->empty())
    {
        serializer.Write(USER_AGENT + " " + UserAgent.get() + "\r\n", stream);
    }

    serializer.Write("\r\n", stream);

    return true;
}

bool RequestSearch::Deserialize(std::istream& stream, Serializer& serializer)
{
    std::string requestString;
    if (!serializer.Read(stream, requestString))
    {
        return false;
    }

    std::vector<std::string> requestLines;
    if (!StringUtility::Split(requestString, std::string("\r\n"), requestLines))
    {
        return false;
    }

    size_t found = std::string::npos;
    for (auto& line : requestLines)
    {
        found = line.find(M_SEARCH);
        if (std::string::npos != found)
        {
            MessageLine.set(StringUtility::Trim(line, std::string(" ")));
            continue;
        }

        found = line.find(HOST);
        if (std::string::npos != found)
        {
            Endpoint.set(StringUtility::Trim(line.substr(found + HOST.length()), std::string(" ")));
            continue;
        }

        found = line.find(MAN);
        if (std::string::npos != found)
        {
            Man.set(StringUtility::Trim(line, std::string(" ")));
            continue;
        }

        found = line.find(MX);
        if (std::string::npos != found)
        {
            Mx.set(std::stoi(line.substr(found + MX.length())));
            continue;
        }

        found = line.find(ST);
        if (std::string::npos != found)
        {
            St.set(StringUtility::Trim(line.substr(found + ST.length()), std::string(" ")));
            continue;
        }

        found = line.find(USER_AGENT);
        if (std::string::npos != found)
        {
            UserAgent.set(StringUtility::Trim(line.substr(found + USER_AGENT.length()), std::string(" ")));
            continue;
        }
    }

    return true;
}

