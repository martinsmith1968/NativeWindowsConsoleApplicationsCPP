#include "stdafx.h"
#include "ListUtils.h"
#include "StringUtils.h"

// ReSharper disable CppInconsistentNaming

using namespace std;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: ListUtils
//--------------------------------------------------------------------------

list<string> ListUtils::ToList(const int argc, char* argv[], const int startAt)
{
    list<string> list;

    for (auto i = startAt; i < argc; ++i)
    {
        list.emplace_back(argv[i]);
    }

    return list;
}

string ListUtils::Find(const list<string>& list, const string& value, bool case_sensitive)
{
    for (auto iter = list.begin(); iter != list.end(); ++iter)
    {
        if (*iter == value)
            return *iter;

        if (!case_sensitive && StringUtils::ToLower(*iter) == StringUtils::ToLower(value))
            return *iter;
    }

    return "";
}

int ListUtils::GetMinLength(const list<string>& list)
{
    if (list.empty())
        return 0;

    const auto max_text_length = min_element(list.begin(),
        list.end(),
        [](const string& a, const string& b) { return (a.length()) < b.length(); }
    )->length();

    return static_cast<int>(max_text_length);
}

int ListUtils::GetMaxLength(const list<string>& list)
{
    if (list.empty())
		return 0;

    const auto max_text_length = max_element(
        list.begin(),
        list.end(),
        [](const string& a, const string& b)
        {
	        return a.length() < b.length();
        }
    )->length();

    return static_cast<int>(max_text_length);
}
