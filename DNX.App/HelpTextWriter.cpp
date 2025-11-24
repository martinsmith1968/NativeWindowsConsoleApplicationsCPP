#include "stdafx.h"
#include "HelpTextWriter.h"

#include "../DNX.Utils/EnvironmentUtils.h"

using namespace DNX::App;
using namespace DNX::Utils;

// ReSharper disable CppInconsistentNaming

HelpTextWriter HelpTextWriter::m_defaultInstance = HelpTextWriter();

HelpTextWriter::HelpTextWriter()
{
    HelpTextWriter::SetMaximumLineWidth(EnvironmentUtils::GetConsoleWindowWidth());
}

HelpTextWriter& HelpTextWriter::GetDefaultInstance()
{
    return m_defaultInstance;
}

size_t HelpTextWriter::GetMaximumLineWidth() const
{
    return m_maximum_line_width;
}

void HelpTextWriter::SetMaximumLineWidth(const size_t maximum_line_width)
{
    m_maximum_line_width = maximum_line_width;
}

list<string> HelpTextWriter::BuildHelpTextLines(const string& text, const int prefix_indent_size) const
{
    list<string> lines;

    lines.push_back(text);

    return lines;
}
