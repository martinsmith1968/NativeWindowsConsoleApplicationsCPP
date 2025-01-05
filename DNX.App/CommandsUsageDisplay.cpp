#include "stdafx.h"
#include "CommandsUsageDisplay.h"

#include <iomanip>

#include "../DNX.Utils/ConsoleUtils.h"
#include "../DNX.Utils/StringUtils.h"
#include <iostream>

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming

using namespace DNX::App;
using namespace DNX::Utils;

string CommandsUsageDisplay::ErrorLinePrefix = "ERROR";

void CommandsUsageDisplay::ShowUsage(const Commands& commands, const AppDetails& appDetails)
{
    cout << appDetails.GetHeaderLine() << std::endl;
    if (!appDetails.Copyright.empty())
    {
        cout << appDetails.Copyright << std::endl;
    }

    cout << std::endl;
    cout << "Usage:" << std::endl;
    cout << AppDetails::GetApplicationName() << " [command-name] { [command-arguments] }" << std::endl;

    size_t max_argument_name_length = 0;
    for (Command& command : commands.GetCommands())
    {
        max_argument_name_length = std::max(command.GetName().length(), max_argument_name_length);
    }

    if (!commands.GetCommands().empty())
    {
        cout << std::endl;
        cout << "Commands:" << std::endl;
        cout << std::endl;
        const auto argument_name_width = max_argument_name_length + 4;
        for (Command& command : commands.GetCommands())
        {
            std::cout << std::left << std::setfill(' ') << std::setw(static_cast<streamsize>(argument_name_width)) << command.GetName()
                << command.GetDescription()
                << std::endl;
        }
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
