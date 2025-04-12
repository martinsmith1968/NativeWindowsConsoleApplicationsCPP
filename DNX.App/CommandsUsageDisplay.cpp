#include "stdafx.h"
#include "CommandsUsageDisplay.h"
#include "../DNX.Utils/ConsoleUtils.h"
#include "../DNX.Utils/FileUtils.h"
#include <iostream>
#include <iomanip>

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
        const auto argument_name_width = max_argument_name_length + 4;
        for (Command& command : command_list)
        {
            std::cout << std::left << std::setfill(' ') << std::setw(static_cast<streamsize>(argument_name_width)) << command.GetName()
                << command.GetDescription()
                << std::endl;
        }
    }

    list<string> argument_file_lines;
    if (parser_config.GetUseDefaultArgumentsFile())
    {
        const auto file_name = AppDetails::GetDefaultArgumentsFileName();

        const auto found = FileUtils::FileExists(file_name);

        argument_file_lines.push_back("Default App arguments can be specified in : " + file_name + (found ? " (exists)" : ""));
    }
    if (parser_config.GetUseLocalArgumentsFile())
    {
        if (AppDetails::GetLocalArgumentsFileName() != AppDetails::GetDefaultArgumentsFileName())
        {
            const auto file_name = AppDetails::GetDefaultArgumentsFileName();

            const auto found = FileUtils::FileExists(file_name);

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
