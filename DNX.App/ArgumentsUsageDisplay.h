#pragma once
#include "stdafx.h"
#include "AppDetails.h"
#include "Arguments.h"
#include "ParserConfig.h"

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
        static string GetArgumentDetails(const Argument& argument, const ParserConfig& parser_config);
        static string GetArgumentDescription(const Argument& argument);

    public:
        static string ErrorLinePrefix;

        static void ShowUsage(const Arguments& arguments, const ParserConfig& parser_config, const AppDetails& appDetails, const string& command_name = "");
        static void ShowErrors(const Arguments& arguments, int blankLinesBefore = 0, int blankLinesAfter = 0);
    };
}
