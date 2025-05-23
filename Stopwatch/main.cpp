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
// - Organise code structure
// - Investigate Execute() method on Commands (base class ?)

//------------------------------------------------------------------------------
int main(const int argc, char* argv[])
{
    try
    {
        const AppInfo appInfo;
        const ParserConfig config;

        AppCommands commands;
        const auto& command = CommandsParser::ParseCommands(commands, argc, argv);

        if (command.IsEmpty())
        {
            CommandsUsageDisplay::ShowUsage(commands, config, appInfo);
            return 1;
        }

        if (!commands.IsValid())
        {
            ArgumentsUsageDisplay::ShowUsage(command.GetArguments(), config, appInfo);
            ArgumentsUsageDisplay::ShowErrors(command.GetArguments(), 1);
            return 2;
        }

        if (command.GetArguments().IsHelp())
        {
            ArgumentsUsageDisplay::ShowUsage(command.GetArguments(), config, appInfo, command.GetName());
            return 3;
        }

        if (!command.GetArguments().IsValid())
        {
            ArgumentsUsageDisplay::ShowUsage(command.GetArguments(), config, appInfo, command.GetName());
            ArgumentsUsageDisplay::ShowErrors(command.GetArguments(), 1);
            return 4;
        }

        // TODO: command->Execute()
        commands.Execute(command);

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
