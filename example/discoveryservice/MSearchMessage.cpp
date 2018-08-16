#include "MSearchMessage.h"
#include "StringUtility.h"

using namespace sg::microreactor;
using namespace sg::service;

const char MSearchMessage::S[] = "S";
const char MSearchMessage::HOST[] = "Host";
const char MSearchMessage::MAN[] = "Man";
const char MSearchMessage::MX[] = "MX";
const char MSearchMessage::ST[] = "ST";


MSearchMessage::MSearchMessage(std::shared_ptr<RestRequest> request)
    : RestMessage(request)
{
    Decode(request);
}

MSearchMessage::~MSearchMessage()
{
}

bool MSearchMessage::GetHeader(const std::string& name, std::string& value) const
{
    const auto found = mHeaders.find(name);
    if (found != mHeaders.end())
    {
        value = found->second;
        return true;
    }

    return false;
}

void MSearchMessage::AddHeader(const std::string& name, const std::string& value)
{
    mHeaders[name] = value;
}

bool MSearchMessage::Decode(std::shared_ptr<RestRequest> request)
{
    if (request == nullptr)
    {
        return false;
    }

    for (const auto& header : request->mHeaders)
    {
        AddHeader(header.mName, header.mValue);
    }

    return true;
}
