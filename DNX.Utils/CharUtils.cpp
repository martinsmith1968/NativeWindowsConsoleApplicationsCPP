#include "stdafx.h"
#include "CharUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyMiscUseAnonymousNamespace
// ReSharper disable CppClangTidyMiscUseInternalLinkage
// ReSharper disable CppTooWideScopeInitStatement

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
