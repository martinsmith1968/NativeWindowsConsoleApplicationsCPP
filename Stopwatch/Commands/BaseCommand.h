#pragma once
#include "../stdafx.h"
#include "CommandType.h"
#include "../TimerRepository.h"
#include "../../DNX.App/Arguments.h"
#include "../../DNX.App/ValueType.h"
#include "../../DNX.Utils/StringUtils.h"
#include <chrono>
#include <string>

#include "../../DNX.Utils/DateUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace Stopwatch
{
    //------------------------------------------------------------------------------
    // Arguments
    class BaseArguments : public Arguments
    {
    protected:
        const int APP_MAX = INT_MAX - 10;

        const string ArgumentNameStopwatchName = "stopwatch-name";
        const string ArgumentNameVerbose       = "verbose";
        const string ArgumentNameFileName      = "filename";

        BaseArguments()
        {
            AddOption(ValueType::STRING, "f", ArgumentNameFileName, TimerRepository::GetDefaultRepositoryFileName(), "The filename to store Stopwatch data in", false, APP_MAX);
        }

        void AddParameterStopwatchName()
        {
            AddParameter(ValueType::STRING, 1, ArgumentNameStopwatchName, "", "The name of the Stopwatch", true);
        }
        void AddSwitchVerboseOutput(const bool default_value)
        {
            AddSwitch("v", ArgumentNameVerbose, StringUtils::BoolToString(default_value), "Control verbosity of output messages", false, 0);
        }

    public:
        string GetFileName() { return GetArgumentValue(ArgumentNameFileName); }

        string GetStopwatchName() { return GetArgumentValue(ArgumentNameStopwatchName); }
        bool GetVerbose() { return GetSwitchValue(ArgumentNameVerbose); }
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

        static string GetElapsedTimeDisplay(const Timer& timer, const string& prefix)
        {
            string text = string(prefix)
                .append(": ")
                .append(timer.GetName())
                .append(" - Elapsed time: ")
                .append(FormatForDisplay(timer.GetAccumulatedElapsed()));

            return text;
        }

        static string GetTimerDetailsDisplay(const Timer& timer, const string& prefix)
        {
            const auto formatted_start_time = FormatForDisplay(timer.GetStartDateTime());

            string text = string(prefix)
                .append(": ")
                .append(timer.GetName())
                .append(" - ")
                .append(formatted_start_time);

            return text;
        }

    public:
        static string FormatForDisplay(const tm& tm)
        {
            return DateUtils::FormatDate(&tm, "%Y-%m-%d %H:%M:%S");
        }

        static string FormatForDisplay(const double timespan)
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

    public:
        [[nodiscard]] CommandType GetCommandType() const { return m_command_type; }
    };
}
