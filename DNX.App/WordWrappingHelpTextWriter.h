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
    // Class: WordWrappingHelpTextWriter
    //--------------------------------------------------------------------------
    class WordWrappingHelpTextWriter : public HelpTextWriter
    {
        static bool IsWrappable(const string& character);
        static bool IsWrappable(char character);

        static WordWrappingHelpTextWriter m_defaultInstance;

    public:
        static WordWrappingHelpTextWriter& GetDefaultInstance();
        [[nodiscard]] list<string> BuildHelpTextLines(const string& text, int prefix_indent_size) const override;
    };
}
