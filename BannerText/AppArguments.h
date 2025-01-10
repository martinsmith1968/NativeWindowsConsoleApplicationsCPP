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
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppTooWideScopeInitStatement

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;

namespace BannerText
{
    enum class TextAlignmentType : uint8_t
    {
        LEFT,
        RIGHT,
        CENTER
    };

    class TextAlignmentTypeTextResolver : public EnumTextResolver<TextAlignmentType>
    {
    public:
        TextAlignmentTypeTextResolver()
        {
            SetText(TextAlignmentType::LEFT, "Left");
            SetText(TextAlignmentType::RIGHT, "Right");
            SetText(TextAlignmentType::CENTER, "Center");
        }
    };

    class AppArguments final : public Arguments
    {
        const string ArgumentNameMessageText        = "message-text";
        const string ArgumentNameHeaderLineChar     = "header-line-char";
        const string ArgumentNameHeaderLineCount    = "header-line-count";
        const string ArgumentNameFooterLineChar     = "footer-line-char";
        const string ArgumentNameFooterLineCount    = "footer-line-count";
        const string ArgumentNameTextLineChar       = "text-line-char";
        const string ArgumentNameTitlePrefixCount   = "title-prefix-count";
        const string ArgumentNameTitleSuffixCount   = "title-suffix-count";
        const string ArgumentNameTitlePrefixGapSize = "title-prefix-gap-size";
        const string ArgumentNameTitleSuffixGapSize = "title-suffix-gap-size";
        const string ArgumentNameTextAlignment      = "text-alignment";
        const string ArgumentNameMinTotalLength     = "min-total-length";
        const string ArgumentNameMaxTotalLength     = "max-total-length";

        TextAlignmentTypeTextResolver TextAlignmentTypeTextConverter;

    public:
        AppArguments()
        {
            AddParameter(ValueType::STRING, 1, ArgumentNameMessageText, "", "The Text to display", true);
            AddOption(ValueType::CHAR, "hlc", ArgumentNameHeaderLineChar, "*", "The character to use for header lines", false);
            AddOption(ValueType::INT, "hln", ArgumentNameHeaderLineCount, "1", "The number of header lines to print", false);
            AddOption(ValueType::CHAR, "flc", ArgumentNameFooterLineChar, "*", "The character to use for footer lines", false);
            AddOption(ValueType::INT, "fln", ArgumentNameFooterLineCount, "1", "The number of footer lines to print", false);
            AddOption(ValueType::CHAR, "tlc", ArgumentNameTextLineChar, "*", "The character to use for text line prefix/suffix", false);
            AddOption(ValueType::INT, "tpc", ArgumentNameTitlePrefixCount, "2", "Set Title Prefix Count", false);
            AddOption(ValueType::INT, "tsc", ArgumentNameTitleSuffixCount, "2", "Set Title Suffix Count", false);
            AddOption(ValueType::INT, "tpgs", ArgumentNameTitlePrefixGapSize, "2", "Set Title Prefix Gap Size", false);
            AddOption(ValueType::INT, "tsgs", ArgumentNameTitleSuffixGapSize, "2", "Set Title Suffix Gap Size", false);
            AddOption(ValueType::ENUM, "ta", ArgumentNameTextAlignment, TextAlignmentTypeTextConverter.GetText(TextAlignmentType::LEFT), "Set Text Alignment", false, 0, TextAlignmentTypeTextConverter.GetAllText());
            AddOption(ValueType::INT, "minl", ArgumentNameMinTotalLength, "0", "Set Minimum Total line length", false);
            AddOption(ValueType::INT, "maxl", ArgumentNameMaxTotalLength, "0", "Set Maximum Total line length", false);
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
            return GetArgumentValue(ArgumentNameMessageText);
        }

        char GetHeaderLineChar()
        {
            return ValueConverter::ToChar(GetArgumentValue(ArgumentNameHeaderLineChar));
        }

        size_t GetHeaderLineCount()
        {
            return ValueConverter::ToInt(GetArgumentValue(ArgumentNameHeaderLineCount));
        }

        char GetFooterLineChar()
        {
            return ValueConverter::ToChar(GetArgumentValue(ArgumentNameFooterLineChar));
        }

        size_t GetFooterLineCount()
        {
            return ValueConverter::ToInt(GetArgumentValue(ArgumentNameFooterLineCount));
        }

        char GetTextLineChar()
        {
            return ValueConverter::ToChar(GetArgumentValue(ArgumentNameTextLineChar));
        }

        size_t GetTitlePrefixCount()
        {
            return ValueConverter::ToInt(GetArgumentValue(ArgumentNameTitlePrefixCount));
        }

        size_t GetTitleSuffixCount()
        {
            return ValueConverter::ToInt(GetArgumentValue(ArgumentNameTitleSuffixCount));
        }

        size_t GetTitlePrefixGapSize()
        {
            return ValueConverter::ToInt(GetArgumentValue(ArgumentNameTitlePrefixGapSize));
        }

        size_t GetTitleSuffixGapSize()
        {
            return ValueConverter::ToInt(GetArgumentValue(ArgumentNameTitleSuffixGapSize));
        }

        TextAlignmentType GetTextAlignment()
        {
            return TextAlignmentTypeTextConverter.GetValue(GetArgumentValue(ArgumentNameTextAlignment));
        }

        size_t GetMinimumTotalLength()
        {
            return ValueConverter::ToInt(GetArgumentValue(ArgumentNameMinTotalLength));
        }

        size_t GetMaximumTotalLength()
        {
            return ValueConverter::ToInt(GetArgumentValue(ArgumentNameMaxTotalLength));
        }

        list<string> GetTextLines()
        {
            const auto textLines = GetTextLinesToOutput();

            list<string> lines;
            for (auto& line : textLines)
            {
                lines.push_back(GetFormattedTextLine(line));
            }

            return lines;
        }

        string GetFormattedTextLine(const string& text)
        {
            auto prefixGapSize = GetTitlePrefixGapSize();
            auto suffixGapSize = GetTitleSuffixGapSize();
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
}
