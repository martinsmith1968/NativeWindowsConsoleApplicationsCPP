#pragma once
#include "stdafx.h"
#include "../DNX.Utils/StringUtils.h"
#include "../DNX.App/Arguments.h"
#include "../DNX.App/Commands.h"
#include "../DNX.App/ValueType.h"
#include <string>
#include <chrono>

#include "TimerRepository.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers

using namespace DNX::Utils;
using namespace Stopwatch;

//------------------------------------------------------------------------------
// Types
enum class CommandType : uint8_t
{
    LIST,
    START,
    STOP,
    PAUSE,
    RESUME,
    ELAPSED,
    DELETE,
};

enum class ListFormatType : uint8_t
{
    DISPLAY,
    CSV,
    CUSTOM
};

class ListFormatTypeText : public EnumTextResolver<ListFormatType>
{
public:
    ListFormatTypeText()
    {
        SetText(ListFormatType::DISPLAY, "Display");
        SetText(ListFormatType::CSV,     "CSV");
        SetText(ListFormatType::CUSTOM,  "Custom");
    }
};

class CommandTypeText : public EnumTextResolver<CommandType>
{
public:
    CommandTypeText()
    {
        SetText(CommandType::LIST   , "List");
        SetText(CommandType::START  , "Start");
        SetText(CommandType::STOP   , "Stop");
        SetText(CommandType::PAUSE,   "Pause");
        SetText(CommandType::RESUME,  "Resume");
        SetText(CommandType::ELAPSED, "Elapsed");
        SetText(CommandType::DELETE , "Delete");
    }
};

//------------------------------------------------------------------------------
// Arguments
class BaseArguments : public Arguments
{
protected:
    const int APP_MAX = INT_MAX - 10;

    const string ArgumentNameStopwatchName = "stopwatch-name";
    const string ArgumentNameFormat        = "output-format";
    const string ArgumentNameVerbose       = "verbose";
    const string ArgumentNameFileName      = "filename";

    BaseArguments()
    {
        AddOption(ValueType::STRING, "f", ArgumentNameFileName, TimerRepository::GetDefaultRepositoryFileName(), "The filename to store Stopwatch data in", false, APP_MAX);
    }

public:
    string GetFileName() { return GetArgumentValue(ArgumentNameFileName); }
};

class ListArguments final : public BaseArguments
{
public:
    ListArguments()
    {
        AddOption(ValueType::STRING, "o", ArgumentNameFormat, ListFormatTypeText().GetText(ListFormatType::DISPLAY), "Control output format of list", false, 0, ListFormatTypeText().GetAllText());
        AddSwitch("v", ArgumentNameVerbose, StringUtils::BoolToString(false), "Control verbosity of output messages", false, 0);
    }

    string GetFormat() { return GetArgumentValue(ArgumentNameFormat); }
    bool GetVerbose() { return GetSwitchValue(ArgumentNameVerbose); }
};

class StartArguments final : public BaseArguments
{

public:
    StartArguments()
    {
        AddParameter(ValueType::STRING, 1, ArgumentNameStopwatchName, "", "The name of the Stopwatch", true);
        AddSwitch("v", ArgumentNameVerbose, StringUtils::BoolToString(true), "Control verbosity of output messages", false, 0);
    }

    string GetStopwatchName() { return GetArgumentValue(ArgumentNameStopwatchName); }
    bool GetVerbose() { return GetSwitchValue(ArgumentNameVerbose); }
};

class StopArguments final : public BaseArguments
{

public:
    StopArguments()
    {
        AddParameter(ValueType::STRING, 1, ArgumentNameStopwatchName, "", "The name of the Stopwatch", true);
        AddSwitch("v", ArgumentNameVerbose, StringUtils::BoolToString(true), "Control verbosity of output messages", false, 0);
    }

    string GetStopwatchName() { return GetArgumentValue(ArgumentNameStopwatchName); }
    bool GetVerbose() { return GetSwitchValue(ArgumentNameVerbose); }
};

//------------------------------------------------------------------------------
// Commands
class AppCommands final : public Commands
{
    ListArguments  _list_arguments;
    StartArguments _start_arguments;
    StopArguments  _stop_arguments;

public:
    AppCommands()
    {
        const CommandTypeText commandTypeText;

        AddCommand(&_list_arguments,  commandTypeText.GetText(CommandType::LIST),  "List the current active Stopwatches");
        AddCommand(&_start_arguments, commandTypeText.GetText(CommandType::START), "Start a named Stopwatch");
        AddCommand(&_stop_arguments,  commandTypeText.GetText(CommandType::STOP),  "Stop a named Stopwatch");
        //AddCommand(AppArgumentsStop(),    "stop",    "Stop an active Timer");
        //AddCommand(AppArgumentsCancel(),  "cancel",  "Cancel an active Timer");
        //AddCommand(AppArgumentsPause(),   "pause",   "Pause an active Timer");
        //AddCommand(AppArgumentsElapsed(), "elapsed", "Get the elapsed time of an active timer");
    }
};
