#pragma once

#include "AppArguments.h"
#include "../DNX.App/SimpleApp.h"

// ReSharper disable CppInconsistentNaming

namespace FigLetText
{
    class App : public SimpleApp<AppArguments>
    {
        static list<string> GetLines(const string& text);
        static Figlet::Banner* GetWriter(FontType fontType);
        static void ApplySpacingType(Figlet::Banner* figlet_writer, SpacingType spacingType);
        static list<string> GenerateUsingFramingType(Figlet::Banner* figlet_writer, FramingType framingType, const string& message_text);
        static int CalculateLeftPadding(TextAlignmentType textAlignmentType, int outputWidth, int max_line_length);
        static list<string> FormatLinesWithPadding(const list<string>& lines, int left_pad);

        static void FormatAndOutputMessage(
            const string& message_text,
            FontType fontType,
            FramingType framingType,
            SpacingType spacingType,
            TextAlignmentType textAlignmentType,
            int outputWidth
        );

    public:
        App(const int argc, char* argv[], const AppDetails& app_details, AppArguments& arguments, const ParserConfig& parser_config)
            : SimpleApp(argc, argv, app_details, arguments, parser_config)
        {
        }

        void Execute(AppArguments& arguments) override;
    };
}
