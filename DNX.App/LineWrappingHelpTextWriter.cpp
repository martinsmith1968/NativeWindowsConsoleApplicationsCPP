#include "stdafx.h"
#include "LineWrappingHelpTextWriter.h"

#include "../DNX.Utils/EnvironmentUtils.h"
#include "../DNX.Utils/StringUtils.h"

using namespace DNX::App;
using namespace DNX::Utils;

// ReSharper disable CppInconsistentNaming

LineWrappingHelpTextWriter LineWrappingHelpTextWriter::m_defaultInstance = LineWrappingHelpTextWriter();

LineWrappingHelpTextWriter& LineWrappingHelpTextWriter::GetDefaultInstance()
{
    return m_defaultInstance;
}

list<string> LineWrappingHelpTextWriter::BuildHelpTextLines(const string& text, const int prefix_indent_size) const
{
    list<string> lines;

    const auto max_text_length = m_maximum_line_width - prefix_indent_size;

    auto remaining = text;

    while (!remaining.empty())
    {
        if (remaining.length() <= max_text_length)
        {
            lines.push_back(remaining);
            remaining.clear();
        }
        else
        {
            auto line = StringUtils::Left(remaining, max_text_length);
            remaining = StringUtils::LTrim(StringUtils::Right(remaining, remaining.length() - max_text_length));
            lines.push_back(line);
        }
    }

    return lines;
}
