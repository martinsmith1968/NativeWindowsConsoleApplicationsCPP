#pragma once

#include <list>
#include <string>
#include "HelpTextWriter.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::App
{
    //--------------------------------------------------------------------------
    // Class: LineWrappingHelpTextWriter
    //--------------------------------------------------------------------------
    class LineWrappingHelpTextWriter : public HelpTextWriter
    {
        static LineWrappingHelpTextWriter m_defaultInstance;

    public:
        static LineWrappingHelpTextWriter& GetDefaultInstance();
        [[nodiscard]] list<string> BuildHelpTextLines(const string& text, int prefix_indent_size) const override;
    };
}
