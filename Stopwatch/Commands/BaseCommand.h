#pragma once
#include "../stdafx.h"
#include "CommandType.h"
#include "../TimerDisplayBuilder.h"
#include "../TimerRepository.h"
#include "../../DNX.App/Arguments.h"
#include "../../DNX.App/ValueType.h"
#include "../../DNX.Utils/StringUtils.h"
#include <chrono>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable StringLiteralTypo

using namespace std;

namespace Stopwatch
{
    //------------------------------------------------------------------------------
    // Arguments
    class BaseArguments : public Arguments
    {
    protected:
        const int APP_MAX = INT_MAX - 10;

        const string ArgumentNameDataFileName             = "data-filename";
        const string ArgumentNameStopwatchName            = "stopwatch-name";
        const string ArgumentNameVerboseOutput            = "verbose-output";
        const string ArgumentNameIgnoreInvalidState       = "ignore-invalid-state";
        const string ArgumentNameShowElapsedTime          = "show-elapsed-time";
        const string ArgumentNameElapsedTimeDisplayFormat = "elapsed-time-display-format";
        const string ArgumentNameAdditionalText           = "additional-text";

        BaseArguments()
        {
            AddOption(ValueType::STRING, "df", ArgumentNameDataFileName, TimerRepository::GetDefaultRepositoryFileName(), "The filename to store Stopwatch data in", false, APP_MAX);
        }

        void AddParameterStopwatchName()
        {
            AddParameter(ValueType::STRING, 1, ArgumentNameStopwatchName, "", "The name of the Stopwatch", true);
        }
        void AddOptionAdditionalText()
        {
            AddOption(ValueType::STRING, "at", ArgumentNameAdditionalText, "", "Additional text for the output", false, APP_MAX - 5);
        }
        void AddSwitchVerboseOutput(const bool default_value)
        {
            AddSwitch("v", ArgumentNameVerboseOutput, StringUtils::BoolToString(default_value), "Control verbosity of output messages", false, APP_MAX - 10);
        }
        void AddSwitchIgnoreInvalidState(const bool default_value)
        {
            AddSwitch("i", ArgumentNameIgnoreInvalidState, StringUtils::BoolToString(default_value), "Ignore errors of Stopwatch being in invalid state for the action", false, APP_MAX - 20);
        }
        void AddSwitchShowElapsedTime(const bool default_value)
        {
            AddSwitch("set", ArgumentNameShowElapsedTime, StringUtils::BoolToString(default_value), "Show the Stopwatch Elapsed Time", false, APP_MAX - 35);
        }
        void AddOptionElapsedTimeDisplayFormat()
        {
            AddOption(ValueType::STRING, "etdf", ArgumentNameElapsedTimeDisplayFormat, "{name}: {state} - " + TimerDisplayBuilder::DefaultElapsedTimeTextFormat, "The format string to use to display Elapsed Time", false, APP_MAX - 34);
        }
        void AddOptionElapsedTimeAlternativeDisplayFormat()
        {
            AddOption(ValueType::STRING, "etdf", ArgumentNameElapsedTimeDisplayFormat, "{name}: {action} - " + TimerDisplayBuilder::DefaultElapsedTimeTextFormat, "The format string to use to display Elapsed Time", false, APP_MAX - 34);
        }

    public:
        string GetDataFileName() { return GetArgumentValue(ArgumentNameDataFileName); }

        string GetStopwatchName() { return GetArgumentValue(ArgumentNameStopwatchName); }
        bool GetVerboseOutput() { return GetSwitchValue(ArgumentNameVerboseOutput); }
        bool GetIgnoreInvalidState() { return GetSwitchValue(ArgumentNameIgnoreInvalidState); }
        bool GetShowElapsedTime() { return GetSwitchValue(ArgumentNameShowElapsedTime); }
        string GetElapsedTimeDisplayFormat() { return GetArgumentValue(ArgumentNameElapsedTimeDisplayFormat); }
        string GetArgumentAdditionalText() { return GetArgumentValue(ArgumentNameAdditionalText); }

    };

    //------------------------------------------------------------------------------
    // Command
    class BaseCommand : public Command
    {
    protected:
        CommandType m_command_type;

        BaseCommand(
            Arguments* arguments,
            const CommandType commandType,
            const string& description,
            const int sequence = 0
        )
            : Command(arguments, CommandTypeTextResolver().GetText(commandType), description, sequence)
        {
            m_command_type = commandType;
        }

        void Execute() override = 0;

        [[noreturn]] static void AbortNotFound(const string& stopwatch_name)
        {
            const string exception_text = "Stopwatch '" + stopwatch_name + "' not found";
            throw exception(exception_text.c_str());
        }

        [[noreturn]] static void AbortAlreadyExists(const string& stopwatch_name)
        {
            const string exception_text = "Stopwatch '" + stopwatch_name + "' already exists";
            throw exception(exception_text.c_str());
        }
        static void AbortInvalidState(const Timer& timer, const CommandType command_type)
        {
            const string exception_text = string("Stopwatch: '")
                .append(timer.GetName())
                .append("' is not in the correct state to ")
                .append(CommandTypeTextResolver().GetText(command_type))
                .append(" - ")
                .append(TimerStateTypeTextResolver().GetText(timer.GetState()));

            throw exception(exception_text.c_str());
        }

        static string GetTimerStatusDisplayText(const Timer& timer, const string& status)
        {
            return timer.GetName() + " " + status;
        }

    public:
        [[nodiscard]] CommandType GetCommandType() const { return m_command_type; }
    };
}
