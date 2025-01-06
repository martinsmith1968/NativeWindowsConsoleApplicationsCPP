#include "stdafx.h"
#include "AppCommands.h"
#include "AppInfo.h"
#include "../DNX.App/ArgumentsParser.h"
#include "../DNX.App/ArgumentsUsageDisplay.h"
#include "../DNX.App/CommandsParser.h"
#include "../DNX.App/CommandsUsageDisplay.h"
#include <iostream>
#include <regex>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppTooWideScope
// ReSharper disable CppClangTidyCertErr33C

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;
using namespace Stopwatch;

//------------------------------------------------------------------------------
// TODO:
// - Remaining Commands
//   - Become functional
// - Custom display formats
//   - Formatter classes with formatter factory
// - Organise code structure
// - Investigate Execute() method on Commands (base class ?)

//------------------------------------------------------------------------------
int main(const int argc, char* argv[])
{
    try
    {
        const AppInfo appInfo;

        AppCommands commands;
        auto& command = CommandsParser::ParseCommands(commands, argc, argv);

        if (command.IsEmpty())
        {
            CommandsUsageDisplay::ShowUsage(commands, appInfo);
            return 1;
        }

        if (!commands.IsValid())
        {
            ArgumentsUsageDisplay::ShowUsage(command.GetArguments(), appInfo);
            ArgumentsUsageDisplay::ShowErrors(command.GetArguments(), 1);
            return 2;
        }

        if (command.GetArguments().IsHelp())
        {
            ArgumentsUsageDisplay::ShowUsage(command.GetArguments(), appInfo, command.GetName());
            return 3;
        }

        if (!command.GetArguments().IsValid())
        {
            ArgumentsUsageDisplay::ShowUsage(command.GetArguments(), appInfo, command.GetName());
            ArgumentsUsageDisplay::ShowErrors(command.GetArguments(), 1);
            return 4;
        }

        commands.Execute(command);
        //command.Execute();

        return 0;
    }
    catch (exception& ex)
    {
        cerr << ArgumentsUsageDisplay::ErrorLinePrefix << ": " << ex.what() << endl;
        return 99;
    }
    catch (...)
    {
        cerr << ArgumentsUsageDisplay::ErrorLinePrefix << ": Unknown error occurred" << endl;
        return 98;
    }
}
