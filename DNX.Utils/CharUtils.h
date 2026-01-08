#pragma once

#include "stdafx.h"
#include <string>

// ReSharper disable CppInconsistentNaming

using namespace std;

namespace DNX::Utils
{
    //--------------------------------------------------------------------------
    // Class: CharUtils
    //--------------------------------------------------------------------------
    class CharUtils
    {
    public:
        static char ToUpper(char ch);
        static char ToLower(char ch);

        static bool IsBetween(char ch, char min, char max);
        static bool IsOneOf(const char ch, char values[]);
        static bool IsOneOf(const char ch, const char* values, const size_t length);
        static bool IsOneOf(const char ch, const string& values);
    };
}
