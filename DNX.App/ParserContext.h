#pragma once
#include "stdafx.h"
#include <string>

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

        static ParserContext _empty_context;

    public:
        ParserContext();
        explicit ParserContext(const string& command_name);

        [[nodiscard]] const string& GetCommandName() const;
        string GetDefaultArgumentsFileName() const;
        string GetLocalArgumentsFileName() const;

        static ParserContext& Empty();
    };
}
