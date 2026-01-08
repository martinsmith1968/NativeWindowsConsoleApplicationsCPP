#include "stdafx.h"
#include "CharUtils.h"
#include "ArrayUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyMiscUseAnonymousNamespace
// ReSharper disable CppClangTidyMiscUseInternalLinkage
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppClangTidyReadabilityUseAnyofallof
// ReSharper disable CppClangTidyReadabilityRedundantStringCstr

using namespace std;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: CharUtils
//--------------------------------------------------------------------------

char CharUtils::ToUpper(const char ch)
{
    return static_cast<char>(toupper(static_cast<unsigned char>(ch)));
}

char CharUtils::ToLower(const char ch)
{
    return static_cast<char>(tolower(static_cast<unsigned char>(ch)));
}

bool CharUtils::IsBetween(const char ch, const char min, const char max)
{
    return ch >= min && ch <= max;
}

bool CharUtils::IsOneOf(const char ch, char values[])
{
    const auto size = ArrayUtils::GetSize(values);

    return IsOneOf(ch, values, size);
}

bool CharUtils::IsOneOf(const char ch, const char* values, const size_t length)
{
    for (size_t x = 0; x < length; ++x)
    {
        if (values[x] == ch)
        {
            return true;
        }
    }

    return false;
}

bool CharUtils::IsOneOf(const char ch, const string& values)
{
    const auto str = values.c_str();
    return IsOneOf(ch, str, values.length());
}
