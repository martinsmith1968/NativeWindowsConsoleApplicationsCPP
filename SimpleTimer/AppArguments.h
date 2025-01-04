#pragma once
#include "stdafx.h"
#include "../DNX.Utils/StringUtils.h"
#include "../DNX.App/Arguments.h"
#include "../DNX.App/Commands.h"
#include <string>
#include <chrono>
#include <iostream>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers

using namespace DNX::Utils;

enum class CommandType : uint8_t
{
    LIST,
    START,
    STOP,
    ELAPSED,
    DELETE,
};

enum class ListFormatType : uint8_t
{
    DISPLAY,
    CSV
};

class ListFormatTypeText : public EnumTextResolver<ListFormatType>
{
public:
    ListFormatTypeText()
    {
        SetText(ListFormatType::DISPLAY, "Display");
        SetText(ListFormatType::CSV, "CSV");
    }
};

//class CommandTypeText : public EnumTextResolver<CommandType>
//{
//public:
//    CommandTypeText()
//    {
//        SetText(CommandType::LIST, "List");
//        SetText(CommandType::START, "Start");
//        SetText(CommandType::STOP, "Stop");
//        SetText(CommandType::ELAPSED, "Elapsed");
//        SetText(CommandType::DELETE, "Delete");
//    }
//};

//------------------------------------------------------------------------------
// Arguments
class AppArgumentsList final : public Arguments
{
    //CommandTypeText CommandTypeTextConverter;

    const string ArgumentNameFormat  = "format";
    const string ArgumentNameVerbose = "verbose";

public:
    AppArgumentsList()
    {
        AddOption(ValueType::STRING, "f", ArgumentNameFormat, ListFormatTypeText().GetText(ListFormatType::DISPLAY), "Control format of list", false, 0, ListFormatTypeText().GetAllText());
        AddSwitch("v", ArgumentNameVerbose, "false", "Control verbosity of output messages", false, 0);
    }

    string GetFormat()
    {
        return GetArgumentValue(ArgumentNameFormat);
    }

    bool GetVerbose()
    {
        return GetSwitchValue(ArgumentNameVerbose);
    }

    void PostParseValidate() override
    {
    }
};

class AppCommands final : public Commands
{
    AppArgumentsList _arguments_list;

public:
    AppCommands()
    {
        AddCommand(&_arguments_list,    "list",    "List the current active Timers");
        //AddCommand(AppArgumentsStart(),   "start",   "Start a named Timer");
        //AddCommand(AppArgumentsStop(),    "stop",    "Stop an active Timer");
        //AddCommand(AppArgumentsCancel(),  "cancel",  "Cancel an active Timer");
        //AddCommand(AppArgumentsPause(),   "pause",   "Pause an active Timer");
        //AddCommand(AppArgumentsElapsed(), "elapsed", "Get the elapsed time of an active timer");
    }
};
