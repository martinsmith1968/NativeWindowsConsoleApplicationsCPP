#pragma once

#include "stdafx.h"
#include "../../libs/DNX.App/Arguments.h"
#include "figlet-1.0/Figlet.hh"
#include <chrono>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppDeclaratorNeverUsed

using namespace std;
using namespace std::chrono;
using namespace DNX::Utils;

// Sources:
// https://ambreen2006.medium.com/exploring-date-and-time-with-chrono-3a8e9af60f62
// https://stackoverflow.com/questions/10654258/get-millisecond-part-of-time
// https://en.cppreference.com/w/cpp/chrono/system_clock
// https://en.cppreference.com/w/cpp/chrono/time_point
namespace FigLetText
{
    enum class FontType : uint8_t
    {
        BANNER,
        BIG,
        DOOM,
        LARRY3D,
        MINI,
        SCRIPT,
        SMALL,
        STANDARD,
        STRAIGHT,
    };

    enum class FramingType : uint8_t
    {
        NONE,
        SINGLE,
        DOUBLE
    };

    enum class SpacingType : uint8_t
    {
        SMUSHED = 0,
        PACKED,
        FULLWIDTH,
        MONOSPACED
    };

    enum class TextAlignmentType : uint8_t
    {
        LEFT,
        RIGHT,
        CENTER
    };

    class FontTypeTextResolver : public EnumTextResolver<FontType>
    {
    public:
        FontTypeTextResolver()
        {
            SetText(FontType::BANNER, "banner");
            SetText(FontType::BIG, "big");
            SetText(FontType::DOOM, "doom");
            SetText(FontType::LARRY3D, "larry3d");
            SetText(FontType::MINI, "mini");
            SetText(FontType::SCRIPT, "script");
            SetText(FontType::SMALL, "small");
            SetText(FontType::STANDARD, "standard");
            SetText(FontType::STRAIGHT, "straight");
        }
    };

    class FramingTypeTextResolver : public EnumTextResolver<FramingType>
    {
    public:
        FramingTypeTextResolver()
        {
            SetText(FramingType::NONE, "none");
            SetText(FramingType::SINGLE, "single");
            SetText(FramingType::DOUBLE, "double");
        }
    };

    class SpacingTypeTextResolver : public EnumTextResolver<SpacingType>
    {
    public:
        SpacingTypeTextResolver()
        {
            SetText(SpacingType::SMUSHED, "smushed");
            SetText(SpacingType::PACKED, "packed");
            SetText(SpacingType::FULLWIDTH, "fullwidth");
            SetText(SpacingType::MONOSPACED, "monospaced");
        }
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

    //------------------------------------------------------------------------------
    // Arguments
    class AppArguments final : public Arguments
    {
        const string ArgumentNameMessageText       = "message-text";
        const string ArgumentNameFontType          = "font-type";
        const string ArgumentNameFramingType       = "framing-type";
        const string ArgumentNameSpacingType       = "spacing-type";
        const string ArgumentNameTextAlignmentType = "text-alignment-type";
        const string ArgumentNameOutputWidth       = "output-width";

        FontTypeTextResolver FontTypeTextConverter;
        FramingTypeTextResolver FramingTypeTextConverter;
        SpacingTypeTextResolver SpacingTypeTextConverter;
        TextAlignmentTypeTextResolver TextAlignmentTypeTextConverter;

    public:
        AppArguments()
        {
            AddParameter(ValueType::STRING, 1, ArgumentNameMessageText, "", "The Text(s) to display", true, list<string>(), true);
            AddOption(ValueType::ENUM, "n", ArgumentNameFontType, FontTypeTextConverter.GetText(FontType::STANDARD), "The font to use", false, 0, FontTypeTextConverter.GetAllText());
            AddOption(ValueType::ENUM, "f", ArgumentNameFramingType, FramingTypeTextConverter.GetText(FramingType::NONE), "The text framing outline to use", false, 0, FramingTypeTextConverter.GetAllText());
            AddOption(ValueType::ENUM, "s", ArgumentNameSpacingType, SpacingTypeTextConverter.GetText(SpacingType::SMUSHED), "The character spacing to use", false, 0, SpacingTypeTextConverter.GetAllText());
            AddOption(ValueType::ENUM, "ta", ArgumentNameTextAlignmentType, TextAlignmentTypeTextConverter.GetText(TextAlignmentType::LEFT), "The Text Alignment", false, 0, TextAlignmentTypeTextConverter.GetAllText());
            AddOption(ValueType::INT, "w", ArgumentNameOutputWidth, "0", "Specify a fixed output width (for alignment)", false);
        }

        list<string> GetMessageTexts()
        {
            return GetArgumentValues(ArgumentNameMessageText);
        }

        FontType GetFontType()
        {
            return FontTypeTextConverter.GetValue(GetArgumentValue(ArgumentNameFontType));
        }

        FramingType GetFramingType()
        {
            return FramingTypeTextConverter.GetValue(GetArgumentValue(ArgumentNameFramingType));
        }

        SpacingType GetSpacingType()
        {
            return SpacingTypeTextConverter.GetValue(GetArgumentValue(ArgumentNameSpacingType));
        }

        TextAlignmentType GetTextAlignmentType()
        {
            return TextAlignmentTypeTextConverter.GetValue(GetArgumentValue(ArgumentNameTextAlignmentType));
        }

        int GetOutputWidth()
        {
            return ValueConverter::ToInt(GetArgumentValue(ArgumentNameOutputWidth));
        }
    };
}
