#include "stdafx.h"
#include "AppInfo.h"
#include "AppArguments.h"
#include "../DNX.Utils/StringUtils.h"
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

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;

//------------------------------------------------------------------------------
// Declarations
namespace SimpleTimer {
    static void List(AppArgumentsList& arguments);  // NOLINT(misc-use-anonymous-namespace)
};

//------------------------------------------------------------------------------
int main(const int argc, char* argv[])
{
    try
    {
        const AppInfo appInfo;

        AppCommands commands;
        const auto& command = CommandsParser::ParseCommands(commands, argc, argv);

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

        if (command.GetName() == "list")
        {
            SimpleTimer::List(dynamic_cast<AppArgumentsList&>(command.GetArguments()));
        }

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

void SimpleTimer::List(AppArgumentsList& arguments)
{
    cout << "Well, shit" << endl;
    cout << "Format: " << arguments.GetFormat() << endl;
    cout << "Verbose: " << arguments.GetVerbose() << endl;
}
