#include "stdafx.h"
#include "App.h"
#include "AppInfo.h"
#include "AppArguments.h"
#include "../DNX.Utils/StringUtils.h"
#include "../DNX.App/ArgumentsParser.h"
#include "../DNX.App/ArgumentsUsageDisplay.h"
#include <iostream>
#include <regex>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;

//------------------------------------------------------------------------------
// main
int main(const int argc, char* argv[])
{
    try
    {
        const AppInfo appInfo;

        AppArguments arguments;
        ArgumentsParser::ParseArguments(arguments, argc, argv);

        if (arguments.IsHelp())
        {
            ArgumentsUsageDisplay::ShowUsage(arguments, appInfo);
            return 1;
        }
        if (!arguments.IsValid())
        {
            ArgumentsUsageDisplay::ShowUsage(arguments, appInfo);
            ArgumentsUsageDisplay::ShowErrors(arguments, 1);
            return 2;
        }

        PauseN::App::Execute(arguments);

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
