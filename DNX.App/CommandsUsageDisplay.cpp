#include "stdafx.h"
#include "CommandsUsageDisplay.h"
#include "../DNX.Utils/ConsoleUtils.h"
#include "../DNX.Utils/FileUtils.h"
#include <iomanip>
#include <iostream>

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming

using namespace DNX::App;
using namespace DNX::Utils;

string CommandsUsageDisplay::ErrorLinePrefix = "ERROR";

void CommandsUsageDisplay::ShowUsage(const Commands& commands, const ParserConfig& parser_config, const AppDetails& appDetails)
{
    cout << appDetails.GetHeaderLine() << std::endl;
    if (!appDetails.Copyright.empty())
    {
        cout << appDetails.Copyright << std::endl;
    }

    cout << std::endl;
    cout << "Usage:" << std::endl;
    cout << AppDetails::GetApplicationName() << " [command-name] { [command-arguments] }" << std::endl;

    auto command_list = commands.GetCommands();
    command_list.sort(Command::CompareBySequence);
    if (!command_list.empty())
    {
        size_t max_argument_name_length = 0;

        for (Command& command : command_list)
        {
            max_argument_name_length = std::max(command.GetName().length(), max_argument_name_length);
        }

        cout << std::endl;
        cout << "Commands:" << std::endl;
        cout << std::endl;
        const auto command_name_width = max_argument_name_length + 2;
        for (Command& command : command_list)
        {
            auto description_lines = parser_config.GetHelpTextWriter()->BuildHelpTextLines(command.GetDescription(), static_cast<int>(command_name_width));

            auto sequence = 0;
            for (auto& line : description_lines)
            {
                ++sequence;
                if (sequence == 1)
                {
                    std::cout << std::left << std::setfill(' ') << std::setw(static_cast<streamsize>(command_name_width)) << command.GetName()
                        << line
                        << std::endl;
                }
                else
                {
                    cout << left << setfill(' ') << setw(static_cast<streamsize>(command_name_width)) << " "
                        << line
                        << endl;
                }
            }

        }
    }

    list<string> argument_file_lines;
    if (parser_config.GetUseDefaultArgumentsFile())
    {
        const auto file_name = AppDetails::GetDefaultArgumentsFileName();

        const auto found = FileUtils::Exists(file_name);

        argument_file_lines.push_back("Default App arguments can be specified in : " + file_name + (found ? " (exists)" : ""));
    }
    if (parser_config.GetUseLocalArgumentsFile())
    {
        if (AppDetails::GetLocalArgumentsFileName() != AppDetails::GetDefaultArgumentsFileName())
        {
            const CommandArguments command_arguments = const_cast<Commands&>(commands).GetArguments();
            const auto file_name = command_arguments.GetParserContext().GetLocalArgumentsFileName();

            const auto found = FileUtils::Exists(file_name);

            argument_file_lines.push_back("Local App arguments can be specified in : " + file_name + (found ? " (exists)" : ""));
        }
    }

    if (!argument_file_lines.empty())
    {
        cout << endl;
        for (const auto& line : argument_file_lines)
            cout << line << endl;
    }
}

void CommandsUsageDisplay::ShowErrors(const Commands& commands, const int blankLinesBefore, const int blankLinesAfter)
{
    ConsoleUtils::ShowBlankLines(blankLinesBefore);

    auto errors = commands.GetErrors();
    for (auto iter = errors.begin(); iter != errors.end(); ++iter)
    {
        cout << ErrorLinePrefix << ": " << *iter << endl;
    }

    ConsoleUtils::ShowBlankLines(blankLinesAfter);
}
