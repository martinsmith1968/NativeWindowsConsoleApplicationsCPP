#include "stdafx.h"
#include "AppInfo.h"
#include "AppArguments.h"
#include "../DNX.Utils/MapUtils.h"
#include "../DNX.Utils/StringUtils.h"
#include "../DNX.App/ArgumentsParser.h"
#include "../DNX.App/ArgumentsUsageDisplay.h"
#include "../DNX.App/CommandsParser.h"
#include "../DNX.App/CommandsUsageDisplay.h"
#include <map>
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

//------------------------------------------------------------------------------
// Declarations
namespace Stopwatch {
    static string FormatForDisplay(const tm& tm);
    static string FormatForDisplay(const double timespan);
    static void List(ListArguments& arguments);     // NOLINT(misc-use-anonymous-namespace)
    static void Start(StartArguments& arguments);   // NOLINT(misc-use-anonymous-namespace)
    static void Stop(StopArguments& arguments);     // NOLINT(misc-use-anonymous-namespace)
};

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

        if (command.GetName() == CommandTypeText().GetText(CommandType::LIST))
        {
            Stopwatch::List(dynamic_cast<ListArguments&>(command.GetArguments()));
        }
        else if (command.GetName() == CommandTypeText().GetText(CommandType::START))
        {
            Stopwatch::Start(dynamic_cast<StartArguments&>(command.GetArguments()));
        }
        else if (command.GetName() == CommandTypeText().GetText(CommandType::STOP))
        {
            Stopwatch::Stop(dynamic_cast<StopArguments&>(command.GetArguments()));
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

string Stopwatch::FormatForDisplay(const tm& tm)
{
    char buffer[20];

    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", &tm);

    return buffer;
}

string Stopwatch::FormatForDisplay(const double timespan)
{
    auto remaining = timespan;

    const auto days = static_cast<int>(floor(remaining / (60 * 60 * 24)));
    if (days > 0)
        remaining -= (days * (60 * 60 * 24));

    const auto hours = static_cast<int>(floor(remaining / (60 * 60)));
    if (hours > 0)
        remaining -= (hours * (60 * 60));

    const auto minutes = static_cast<int>(floor(remaining / 60));
    if (minutes > 0)
        remaining -= (minutes * 60);

    const auto seconds = static_cast<int>(remaining);

    string display;

    if (days > 0)
    {
        if (!display.empty())
            display.append(", ");

        display
            .append(to_string(days))
            .append(" days");
    }

    if (hours > 0)
    {
        if (!display.empty())
            display.append(", ");

        display
            .append(to_string(hours))
            .append(" hours");
    }

    if (minutes > 0)
    {
        if (!display.empty())
            display.append(", ");

        display
            .append(to_string(minutes))
            .append(" minutes");
    }

    if (!display.empty())
        display.append(", ");

    display
        .append(to_string(seconds))
        .append(" seconds");

    return display;
}

void Stopwatch::List(ListArguments& arguments)
{
    const auto repository = TimerRepository(arguments.GetFileName());

    const auto timers = repository.ReadAll();

    for (const auto& [key, value] : timers)
    {
        cout << value.GetName() << "  " << FormatForDisplay(value.GetStartDateTime()) << " - " << FormatForDisplay(value.GetAccumulatedElapsed())  << endl;
    }
}

void Stopwatch::Start(StartArguments& arguments)
{
    const auto stopwatch_name = arguments.GetStopwatchName();

    auto repository = TimerRepository(arguments.GetFileName());

    auto timers = repository.ReadAll();
    if (MapUtils::Exists(timers, stopwatch_name)) // timers.contains(stopwatch_name) != timers.end())
    {
        const string exception_text = stopwatch_name + " already exists";
        throw exception(exception_text.c_str());
    }

    auto timer = Timer(stopwatch_name);
    timer.Start();

    timers.emplace(timer.GetName(), timer);
    repository.SaveAll(timers);

    if (arguments.GetVerbose())
    {
        const auto start_time = timer.GetStartDateTime();
        const auto formatted_start_time = FormatForDisplay(start_time);
        cout << "Started: " << stopwatch_name << " - " << formatted_start_time << endl;
    }
}

void Stopwatch::Stop(StopArguments& arguments)
{
    const auto stopwatch_name = arguments.GetStopwatchName();

    auto repository = TimerRepository(arguments.GetFileName());

    auto timers = repository.ReadAll();
    if (timers.find(stopwatch_name) == timers.end())
    {
        const string exception_text = stopwatch_name + " not found";
        throw exception(exception_text.c_str());
    }

    auto timer = timers.at(stopwatch_name);
    timer.Stop();

    if (arguments.GetVerbose())
    {

        const auto start_time = timer.GetStartDateTime();
        const auto formatted_start_time = FormatForDisplay(start_time);
        cout << timer.GetName() << "  " << FormatForDisplay(timer.GetStartDateTime()) << " - " << FormatForDisplay(timer.GetAccumulatedElapsed()) << endl;
    }

    timers.erase(timers.find(stopwatch_name));

    repository.SaveAll(timers);
}
