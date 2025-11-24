#include "stdafx.h"
#include "WordWrappingHelpTextWriter.h"

#include "../DNX.Utils/EnvironmentUtils.h"
#include "../DNX.Utils/StringUtils.h"

using namespace DNX::App;
using namespace DNX::Utils;

// ReSharper disable CppInconsistentNaming

WordWrappingHelpTextWriter WordWrappingHelpTextWriter::m_defaultInstance = WordWrappingHelpTextWriter();

WordWrappingHelpTextWriter& WordWrappingHelpTextWriter::GetDefaultInstance()
{
    return m_defaultInstance;
}

bool WordWrappingHelpTextWriter::IsWrappable(const string& character)
{
    return IsWrappable(character[0]);
}
bool WordWrappingHelpTextWriter::IsWrappable(const char character)
{
    return !std::isalpha(character) && character != '.';
}

list<string> WordWrappingHelpTextWriter::BuildHelpTextLines(const string& text, const int prefix_indent_size) const
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

            while (!line.empty() && !IsWrappable(StringUtils::Right(line, 1)) && !IsWrappable(StringUtils::Right(line, 1)))
            {
                remaining = StringUtils::LTrim(StringUtils::Right(line, 1) + remaining);
                line = StringUtils::Left(line, line.length() - 1);
            }

            lines.push_back(line);
        }
    }

    return lines;
}
