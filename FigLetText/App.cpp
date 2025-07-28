#include "stdafx.h"
#include "App.h"
#include "../DNX.Utils/ConsoleUtils.h"
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

list<string> GetLines(const string& text)
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
// Execute
void App::Execute(AppArguments& arguments)
{
    const auto message_text      = arguments.GetMessageText();
    const auto fontType          = arguments.GetFontType();
    const auto framingType       = arguments.GetFramingType();
    const auto spacingType       = arguments.GetSpacingType();
    const auto textAlignmentType = arguments.GetTextAlignmentType();
    auto outputWidth             = arguments.GetOutputWidth();

    if (outputWidth <= 0)
    {
        outputWidth = ConsoleUtils::GetConsoleWidth();
    }

    std::ostringstream output_stream;

    Figlet::Banner* figlet_writer = nullptr;
    switch (fontType)
    {
        case FontType::BANNER:
            figlet_writer = &Figlet::banner;
            break;
        case FontType::BIG:
            figlet_writer = &Figlet::big;
            break;
        case FontType::DOOM:
            figlet_writer = &Figlet::doom;
            break;
        case FontType::LARRY3D:
            figlet_writer = &Figlet::larry3d;
            break;
        case FontType::MINI:
            figlet_writer = &Figlet::mini;
            break;
        case FontType::SCRIPT:
            figlet_writer = &Figlet::script;
            break;
        case FontType::SMALL:
            figlet_writer = &Figlet::small;
            break;
        case FontType::STANDARD:
            figlet_writer = &Figlet::standard;
            break;
        case FontType::STRAIGHT:
            figlet_writer = &Figlet::straight;
            break;
        default:
            figlet_writer = &Figlet::standard;
            break;
    }

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


    auto lines = GetLines(output_stream.str());
    const auto max_line_length = std::max_element(lines.begin(),
        lines.end(),
        [](const string& a, const string& b) { return a.length() < b.length(); }
    )->length();

    // TODO: Handle output width properly

    switch (textAlignmentType)
    {
        case TextAlignmentType::CENTER:
            // TODO: Adjust lines for Alignment
            break;
        case TextAlignmentType::RIGHT:
            // TODO: Adjust lines for Alignment
            break;
        case TextAlignmentType::LEFT:
            break;
        default:
            break;
    }

    cout << output_stream.str();
}
