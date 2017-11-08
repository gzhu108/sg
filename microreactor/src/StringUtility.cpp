#include "StringUtility.h"

using namespace sg::microreactor;


bool StringUtility::CompareStringICase(const std::string& str1, const std::string& str2)
{
    if (str1.size() != str2.size()) 
    {
        return false;
    }

    for (std::string::const_iterator c1 = str1.begin(), c2 = str2.begin(); c1 != str1.end(); ++c1, ++c2) 
    {
        if (tolower(*c1) != tolower(*c2)) 
        {
            return false;
        }
    }

    return true;
}

bool StringUtility::CompareStringICase(const std::wstring& str1, const std::wstring& str2)
{
    if (str1.size() != str2.size()) 
    {
        return false;
    }

    for (std::wstring::const_iterator c1 = str1.begin(), c2 = str2.begin(); c1 != str1.end(); ++c1, ++c2) 
    {
        if (towlower(*c1) != towlower(*c2)) 
        {
            return false;
        }
    }

    return true;
}

