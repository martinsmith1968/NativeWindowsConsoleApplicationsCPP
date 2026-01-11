#pragma once

#include "stdafx.h"
#include <list>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyClangDiagnosticPadded
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions
// ReSharper disable CppClangTidyClangDiagnosticDeprecatedCopyWithDtor

using namespace std;

namespace DNX::App
{
    //--------------------------------------------------------------------------
    // Class: HelpTextWriter
    //--------------------------------------------------------------------------
    class HelpTextWriter {

        static HelpTextWriter m_defaultInstance;

    protected:
        size_t m_maximum_line_width = 80;

    public:
        HelpTextWriter();
        virtual ~HelpTextWriter() = default;
        static HelpTextWriter& GetDefaultInstance();

        [[nodiscard]] size_t GetMaximumLineWidth() const;
        virtual void SetMaximumLineWidth(size_t maximum_line_width);

        [[nodiscard]] virtual list<string> BuildHelpTextLines(const string& text, int prefix_indent_size) const;
    };
}
