#pragma once
#include "stdafx.h"
#include <string>

#include "AppDetails.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::App
{
    //--------------------------------------------------------------------------
    // Class: ParserConfig
    //--------------------------------------------------------------------------
    class ParserContext {
        string _command_name;

    public:
        ParserContext()
            : ParserContext("")
        {
        }

        explicit ParserContext(const string& command_name)
        {
            _command_name = command_name;
        }

        [[nodiscard]] const string& GetCommandName() const
        {
            return _command_name;
        }

        string GetDefaultArgumentsFileName() const
        {
            if (_command_name.empty())
                return AppDetails::GetDefaultArgumentsFileName();
            return AppDetails::GetDefaultCommandArgumentsFileName(_command_name);
        }

        string GetLocalArgumentsFileName() const
        {
            if (_command_name.empty())
                return AppDetails::GetLocalArgumentsFileName();
            return AppDetails::GetLocalCommandArgumentsFileName(_command_name);
        }
    };
}
