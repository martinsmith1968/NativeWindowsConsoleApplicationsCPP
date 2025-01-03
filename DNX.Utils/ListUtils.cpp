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

    for (auto i=startAt; i < argc; ++i)
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
