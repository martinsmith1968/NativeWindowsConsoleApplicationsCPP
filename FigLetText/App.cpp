#include "stdafx.h"
#include "App.h"
#include "../DNX.Utils/ConsoleUtils.h"
#include "../DNX.Utils/EnvironmentUtils.h"
#include "../DNX.Utils/ListUtils.h"
#include <chrono>
#include <iostream>
#include <regex>
#include <sstream>

using namespace std;
using namespace std::chrono;
using namespace FigLetText;
using namespace DNX::Utils;

// ReSharper disable CppTooWideScope
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppClangTidyConcurrencyMtUnsafe
// ReSharper disable CppClangTidyClangDiagnosticCoveredSwitchDefault

//------------------------------------------------------------------------------
list<string> App::GetLines(const string& text)
{
    list<string> lines;
    stringstream ss(text);
    string line;
    while (getline(ss, line))
    {
        if (!line.empty())
        {
            lines.push_back(line);
        }
    }
    return lines;
}

//------------------------------------------------------------------------------
Figlet::Banner* App::GetWriter(const FontType fontType)
{
	switch (fontType)
	{
	case FontType::BANNER:
		return &Figlet::banner;
		break;
	case FontType::BIG:
		return &Figlet::big;
		break;
	case FontType::DOOM:
		return &Figlet::doom;
		break;
	case FontType::LARRY3D:
		return &Figlet::larry3d;
		break;
	case FontType::MINI:
		return &Figlet::mini;
		break;
	case FontType::SCRIPT:
		return &Figlet::script;
		break;
	case FontType::SMALL:
		return &Figlet::small;
		break;
	case FontType::STANDARD:
		return &Figlet::standard;
		break;
	case FontType::STRAIGHT:
		return &Figlet::straight;
		break;
	default:
		return &Figlet::standard;
		break;
	}
}

//------------------------------------------------------------------------------
void App::ApplySpacingType(Figlet::Banner* figlet_writer, const SpacingType spacingType)
{
	switch (spacingType)
	{
	case SpacingType::FULLWIDTH:
		figlet_writer->setFullWidth();
		break;
	case SpacingType::PACKED:
		figlet_writer->setPacked();
		break;
	case SpacingType::MONOSPACED:
		figlet_writer->setMonospaced();
		break;
	case SpacingType::SMUSHED:
		figlet_writer->setSmushed();
		break;
	default:
		figlet_writer->setSmushed();
		break;
	}
}

//------------------------------------------------------------------------------
list<string> App::GenerateUsingFramingType(Figlet::Banner* figlet_writer, const FramingType framingType, const string& message_text)
{
    std::ostringstream output_stream;

	switch (framingType)
	{
	case FramingType::SINGLE:
		figlet_writer->printFramed(message_text.c_str(), output_stream, Figlet::FIGLET_SINGLE);
		break;
	case FramingType::DOUBLE:
		figlet_writer->printFramed(message_text.c_str(), output_stream, Figlet::FIGLET_DOUBLE);
		break;
	case FramingType::NONE:
		figlet_writer->print(message_text.c_str(), output_stream);
		break;
	default:
		figlet_writer->print(message_text.c_str(), output_stream);
		break;
	}

	auto output_lines = GetLines(output_stream.str());

	return output_lines;
}

int App::CalculateLeftPadding(const TextAlignmentType textAlignmentType, const int outputWidth, const int max_line_length)
{
	int left_pad = 0;
	if (outputWidth > 0)
	{
		switch (textAlignmentType)
		{
		case TextAlignmentType::CENTER:
			left_pad = (outputWidth - max_line_length) / 2;
			break;
		case TextAlignmentType::RIGHT:
			left_pad = outputWidth - max_line_length - static_cast<int>(EnvironmentUtils::GetNewLine().length() - 1);
			break;
		case TextAlignmentType::LEFT:
			break;
		default:
			break;
		}
	}

	return left_pad;
}

list<string> App::FormatLinesWithPadding(const list<string>& lines, const int left_pad)
{
	auto formatted_lines = list<string>();

	for (auto line : lines)
	{
		line = StringUtils::RTrim(line, ' ');

		if (left_pad == 0)
		{
			formatted_lines.emplace_back(line);
			continue;
		}

		const auto formatted_line = StringUtils::Repeat(" ", left_pad) + line;
		formatted_lines.emplace_back(formatted_line);
	}

	return formatted_lines;
}

//------------------------------------------------------------------------------
void App::FormatAndOutputMessage(
	const string& message_text,
    const FontType fontType,
    const FramingType framingType,
    const SpacingType spacingType,
    const TextAlignmentType textAlignmentType,
	const int outputWidth
)
{
    Figlet::Banner* figlet_writer = GetWriter(fontType);

    ApplySpacingType(figlet_writer, spacingType);

    const auto output_lines = GenerateUsingFramingType(figlet_writer, framingType, message_text);

    const auto max_line_length = ListUtils::GetMaxLength(output_lines);

    const auto left_pad = CalculateLeftPadding(textAlignmentType, outputWidth, max_line_length);

    const auto formatted_lines = FormatLinesWithPadding(output_lines, left_pad);

    for (const auto& formatted_line : formatted_lines)
    {
		cout << formatted_line << endl;
    }
}

//------------------------------------------------------------------------------
void App::Execute(AppArguments& arguments)
{
    const auto message_texts = arguments.GetMessageTexts();
    const auto fontType = arguments.GetFontType();
    const auto framingType = arguments.GetFramingType();
    const auto spacingType = arguments.GetSpacingType();
    const auto textAlignmentType = arguments.GetTextAlignmentType();
    auto outputWidth = arguments.GetOutputWidth();

    if (outputWidth <= 0)
    {
        outputWidth = ConsoleUtils::GetConsoleWidth();
    }

    for (auto& message_text : message_texts)
    {
        FormatAndOutputMessage(message_text, fontType, framingType, spacingType, textAlignmentType, outputWidth);
    }
}
