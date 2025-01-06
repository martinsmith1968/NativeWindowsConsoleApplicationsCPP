#pragma once
#include "stdafx.h"
#include "AppDetails.h"
#include "Arguments.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions
// ReSharper disable CppClassCanBeFinal

namespace DNX::App
{
    //--------------------------------------------------------------------------
    // Class: ArgumentsUsageDisplay
    //--------------------------------------------------------------------------
    class ArgumentsUsageDisplay
    {
    public:
        static string ErrorLinePrefix;

        static void ShowUsage(const Arguments& arguments, const AppDetails& appDetails, const string& command_name = "");
        static void ShowErrors(const Arguments& arguments, int blankLinesBefore = 0, int blankLinesAfter = 0);
    };
}
