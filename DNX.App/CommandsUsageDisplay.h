#pragma once
#include "stdafx.h"
#include "AppDetails.h"
#include "Commands.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions
// ReSharper disable CppClassCanBeFinal

namespace DNX::App
{
    //--------------------------------------------------------------------------
    // Class: CommandsUsageDisplay
    //--------------------------------------------------------------------------
    class CommandsUsageDisplay
    {
    public:
        static string ErrorLinePrefix;

        static void ShowUsage(const Commands& commands, const AppDetails& appDetails);
        static void ShowErrors(const Commands& commands, int blankLinesBefore = 0, int blankLinesAfter = 0);
    };
}
