#pragma once
#ifndef sg_microreactor_StringUtility
#define sg_microreactor_StringUtility

#include <vector>
#include <locale>
#include <algorithm>
#include <regex>
#include <chrono>
#include <iomanip>
#include "Common.h"


namespace sg { namespace microreactor
{
    namespace StringUtility
    {
        enum : int32_t
        {
            Unspecified = -1,
        };

        template <typename String>
        String& ToUpper(String& original)
        {
            std::transform(original.begin(), original.end(), original.begin(), [](typename String::value_type c)->typename String::value_type
            {
                return (typename String::value_type)::toupper((int)c);
            });

            return original;
        }

        template <typename String>
        String GetUpper(const String& original)
        {
            String destination(original);
            return ToUpper(destination);
        }

        template <typename String>
        String& ToLower(String& original)
        {
            std::transform(original.begin(), original.end(), original.begin(), [](typename String::value_type c)->typename String::value_type
            {
                return (typename String::value_type)::tolower((int)c);
            });

            return original;
        }

        template <typename String>
        String GetLower(const String& original)
        {
            String destination(original);
            return ToLower(destination);
        }

        template<typename String>
        bool Split(const String& str, const std::regex& regex, std::vector<String>& parts)
        {
            // std::regex regex{R"([\s,]+)"}; // split on space and comma
            std::sregex_token_iterator it{str.begin(), str.end(), regex, -1};
            parts.assign(it, {});
            return !parts.empty();
        }

        template<typename String>
        bool Split(const String& str, const String& delimiters, std::vector<String>& parts, int32_t totalSplits = Unspecified, bool ignoreEmpty = true)
        {
            bool status = false;

            if (((totalSplits != Unspecified) && (totalSplits < 2)) || (delimiters.length() == 0))
            {
                return status;
            }

            size_t offset = 0;
            size_t eol = String::npos;

            do
            {
                bool emptypart = false;
                eol = str.find(delimiters, offset);
                bool noMoreDelimiters = eol == String::npos;
                bool noMoreSplits = (totalSplits != -1) && (parts.size() == size_t(totalSplits - 1));
                if (!noMoreDelimiters && !noMoreSplits)
                {
                    String line = str.substr(offset, eol - offset);
                    offset = eol + delimiters.length();
                    emptypart = line.length() == 0;
                    if (!(emptypart && ignoreEmpty))
                    {
                        parts.emplace_back(line);
                        status = true;
                    }
                }
                else
                {
                    String remainder = str.substr(offset, str.length() - offset);
                    if (remainder.length() > 0)
                    {
                        parts.emplace_back(remainder);
                        status = true;
                        break;
                    }
                }
            } while (eol != String::npos);

            return status;
        }
        
        template<typename String>
        String Trim(const String& str, const String& trimmedChars)
        {
            if ((str.length() == 0))
            {
                return String();
            }

            String trimmedStr = str;
            size_t first = str.find_first_not_of(trimmedChars);
            if (String::npos == first)
            {
                trimmedStr = str.substr(first, str.find_last_not_of(trimmedChars) - first + 1);
            }

            return trimmedStr;
        }

        inline std::string GetHttpTimeString()
        {
            std::stringstream stream;
            time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            stream << std::put_time(std::gmtime(&now), "%a, %d %b %Y %T %Z");
            return stream.str();
        }

        bool CompareStringICase(const std::string& l, const std::string& r);
        bool CompareStringICase(const std::wstring& l, const std::wstring& r);
    }
}}

#endif // sg_microreactor_StringUtility
