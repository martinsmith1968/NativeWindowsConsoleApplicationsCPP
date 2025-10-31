#pragma once

#include "stdafx.h"
#include <cctype>

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
    };
}
