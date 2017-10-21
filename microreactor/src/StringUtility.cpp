#include "StringUtility.h"

using namespace sg::microreactor;


/*
bool StringUtility::Convert(const std::string& source, std::wstring& target)
{
    // For UTF8, the flag must be zero or MB_ERR_INVALID_CHARS
    int requiredSize = MultiByteToWideChar(CP_UTF8, 0, source.c_str(), (int)source.length(), nullptr, 0);
    if (requiredSize != 0)
    {
        target.resize(requiredSize);
        requiredSize = MultiByteToWideChar(CP_UTF8, 0, source.c_str(), (int)source.length(), &target[0], requiredSize);
        if (requiredSize != 0)
        {
            return true;
        }
    }

    return false;
}

bool StringUtility::Convert(const std::wstring& source, std::string& target)
{
    // For UTF8, the flag must be zero or MB_ERR_INVALID_CHARS
    int requiredSize = WideCharToMultiByte(CP_UTF8, 0, source.c_str(), (int)source.length(), nullptr, 0, nullptr, nullptr);
    if (requiredSize != 0)
    {
        target.resize(requiredSize);
        requiredSize = WideCharToMultiByte(CP_UTF8, 0, source.c_str(), (int)source.length(), &target[0], requiredSize, nullptr, nullptr);
        if (requiredSize != 0)
        {
            return true;
        }
    }
    
    return false;
}

bool StringUtility::GetHttpDateString(std::wstring& httpDate)
{
    SYSTEMTIME st;
    GetSystemTime(&st);

    // Format date
    wchar_t date[32];
    int dateWritten = GetDateFormatEx(L"en-US", 0, &st, L"ddd',' dd MMM yyyy", date, 32, nullptr);
    if (!dateWritten)
    {
        return false;
    }

    // Format time
    wchar_t time[32];
    int timeWritten = GetTimeFormatEx(L"en-US", TIME_FORCE24HOURFORMAT, &st, L" HH':'mm':'ss", time, 32);
    if (!timeWritten)
    {
        return false;
    }

    // Add GMT
    httpDate = date;
    httpDate += time;
    httpDate += L" GMT";
    return true;
}

bool StringUtility::GetHttpDateString(std::string& httpDate)
{
    std::wstring wHttpDate;
    if (GetHttpDateString(wHttpDate))
    {
        return Convert(wHttpDate, httpDate);
    }
    return false;
}
*/

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

