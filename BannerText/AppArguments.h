#pragma once
#include "stdafx.h"
#include <algorithm>
#include <string>
#include "../DNX.Utils/StringUtils.h"
#include "../DNX.App/Arguments.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable StringLiteralTypo
// ReSharper disable CppClangTidyModernizeReturnBracedInitList
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;

enum class TextAlignmentType : uint8_t
{
    LEFT,
    RIGHT,
    CENTER
};

class TextAlignmentTypeText : public EnumTextResolver<TextAlignmentType>
{
    public:
    TextAlignmentTypeText()
    {
        SetText(TextAlignmentType::LEFT, "Left");
        SetText(TextAlignmentType::RIGHT, "Right");
        SetText(TextAlignmentType::CENTER, "Center");
    }
};

class AppArguments final : public Arguments
{
    TextAlignmentTypeText TextAlignmentTypeTextConverter;

    public:
    AppArguments()
    {
        AddParameter(ValueType::STRING, 1, "message-text", "", "The Text to display", true);
        AddOption(ValueType::CHAR, "hlc", "header-line-char", "*", "The character to use for header lines", false);
        AddOption(ValueType::INT, "hln", "header-line-count", "1", "The number of header lines to print", false);
        AddOption(ValueType::CHAR, "flc", "footer-line-char", "*", "The character to use for footer lines", false);
        AddOption(ValueType::INT, "fln", "footer-line-count", "1", "The number of footer lines to print", false);
        AddOption(ValueType::CHAR, "tlc", "text-line-char", "*", "The character to use for text line prefix/suffix", false);
        AddOption(ValueType::INT, "tpc", "title-prefix-count", "2", "Set Title Prefix Count", false);
        AddOption(ValueType::INT, "tsc", "title-suffix-count", "2", "Set Title Suffix Count", false);
        AddOption(ValueType::INT, "tpgs", "title-prefix-gap-size", "2", "Set Title Prefix Gap Size", false);
        AddOption(ValueType::INT, "tsgs", "title-suffix-gap-size", "2", "Set Title Suffix Gap Size", false);
        AddOption(ValueType::ENUM, "ta", "text-alignment", "CENTER", "Set Text Alignment", false, 0, TextAlignmentTypeTextConverter.GetAllText());
        AddOption(ValueType::INT, "minl", "min-total-length", "0", "Set Minimum Total line length", false);
        AddOption(ValueType::INT, "maxl", "max-total-length", "0", "Set Maximum Total line length", false);
    }

    void PostParseValidate() override
    {
        if (!IsValid())
            return;

        const auto printableLineLength = GetTextPrintableLineLength();
        if (printableLineLength <= 0)
        {
            AddError(string("Maximum line length too restrictive: ") + to_string(printableLineLength));
        }
    }

    string GetMessageText()
    {
        return GetArgumentValue("message-text");
    }

    char GetHeaderLineChar()
    {
        return ValueConverter::ToChar(GetArgumentValue("hlc"));
    }

    size_t GetHeaderLineCount()
    {
        return ValueConverter::ToInt(GetArgumentValue("hln"));
    }

    char GetFooterLineChar()
    {
        return ValueConverter::ToChar(GetArgumentValue("flc"));
    }

    size_t GetFooterLineCount()
    {
        return ValueConverter::ToInt(GetArgumentValue("fln"));
    }

    char GetTextLineChar()
    {
        return ValueConverter::ToChar(GetArgumentValue("tlc"));
    }

    size_t GetTitlePrefixCount()
    {
        return ValueConverter::ToInt(GetArgumentValue("tpc"));
    }

    size_t GetTitleSuffixCount()
    {
        return ValueConverter::ToInt(GetArgumentValue("tsc"));
    }

    size_t GetTitlePrefixGapSize()
    {
        return ValueConverter::ToInt(GetArgumentValue("tpgs"));
    }

    size_t GetTitleSuffixGapSize()
    {
        return ValueConverter::ToInt(GetArgumentValue("tsgs"));
    }

    TextAlignmentType GetTextAlignment()
    {
        return TextAlignmentTypeTextConverter.GetValue(GetArgumentValue("ta"));
    }

    size_t GetMinimumTotalLength()
    {
        return ValueConverter::ToInt(GetArgumentValue("minl"));
    }

    size_t GetMaximumTotalLength()
    {
        return ValueConverter::ToInt(GetArgumentValue("maxl"));
    }

    list<string> GetTextLines()
    {
        auto textLines = GetTextLinesToOutput();

        list<string> lines;
        for (auto iter = textLines.begin(); iter != textLines.end(); ++iter)
        {
            lines.push_back(GetFormattedTextLine(*iter));
        }

        return lines;
    }

    string GetFormattedTextLine(const string& text)
    {
        auto prefixGapSize    = GetTitlePrefixGapSize();
        auto suffixGapSize    = GetTitleSuffixGapSize();
        const auto textLength = text.length();

        if (GetMinimumTotalLength() > 0 || GetMaximumTotalLength() > 0)
        {
            switch (GetTextAlignment())
            {
                case TextAlignmentType::LEFT:
                    suffixGapSize = GetLineLength() - textLength - GetTitlePrefixCount() - GetTitlePrefixGapSize() - GetTitleSuffixCount();
                    break;
                case TextAlignmentType::RIGHT:
                    prefixGapSize = GetLineLength() - textLength - GetTitlePrefixCount() - GetTitleSuffixGapSize() - GetTitleSuffixCount();
                    break;
                case TextAlignmentType::CENTER:
                    const auto totalGapSize = GetLineLength() - textLength - GetTitlePrefixCount() - GetTitleSuffixCount();
                    prefixGapSize = totalGapSize / 2;
                    suffixGapSize = totalGapSize - prefixGapSize;
                    break;
            }
        }

        return GetTitlePrefixText()
               .append(string(prefixGapSize, ' '))
               .append(text)
               .append(string(suffixGapSize, ' '))
               .append(GetTitleSuffixText());
    }

    string GetHeaderLine()
    {
        return string(GetLineLength(), GetHeaderLineChar());
    }

    string GetFooterLine()
    {
        return string(GetLineLength(), GetFooterLineChar());
    }

    protected:
    size_t GetLineLength()
    {
        auto lineLength = GetMessageText().length()
            + (GetTitlePrefixCount() + GetTitlePrefixGapSize())
            + (GetTitleSuffixCount() + GetTitleSuffixGapSize());

        if (GetMinimumTotalLength() > 0)
        {
            lineLength = std::max(lineLength, GetMinimumTotalLength());
        }

        if (GetMaximumTotalLength() > 0)
        {
            lineLength = std::min(lineLength, GetMaximumTotalLength());
        }

        return lineLength;
    }

    size_t GetTextPrintableLineLength()
    {
        return GetLineLength()
            - GetTitlePrefixCount() - GetTitlePrefixGapSize()
            - GetTitleSuffixCount() - GetTitleSuffixCount();
    }

    list<string> GetTextLinesToOutput()
    {
        list<string> lines;

        const auto lineLength = GetTextPrintableLineLength();

        auto remainingText = GetMessageText();
        while (!remainingText.empty())
        {
            if (remainingText.length() > lineLength)
            {
                lines.push_back(remainingText.substr(0, lineLength));
                remainingText = remainingText.substr(lineLength, string::npos);
            }
            else
            {
                lines.push_back(remainingText);
                remainingText.clear();
            }
        }

        return lines;
    }

    string GetTitlePrefixText()
    {
        return string(GetTitlePrefixCount(), GetTextLineChar());
    }

    string GetTitleSuffixText()
    {
        return string(GetTitleSuffixCount(), GetTextLineChar());
    }
};
