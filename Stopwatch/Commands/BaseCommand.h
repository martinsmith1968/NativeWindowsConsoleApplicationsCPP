#pragma once

#include "CommandType.h"
#include "../TimerDisplayBuilder.h"
#include "../TimerRepository.h"
#include "../stdafx.h"
#include "../../DNX.App/Arguments.h"
#include "../../DNX.App/Command.h"
#include <chrono>
#include <iostream>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable StringLiteralTypo

namespace DNX::App
{
    class Command;
}

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
        const string ArgumentNameQuiet                    = "quiet";
        const string ArgumentNameIgnoreInvalidState       = "ignore-invalid-state";
        const string ArgumentNameShowElapsedTime          = "show-elapsed-time";
        const string ArgumentNameElapsedTimeDisplayFormat = "elapsed-time-display-format";
        const string ArgumentNameAdditionalText           = "additional-text";

        explicit BaseArguments(const ParserContext& parser_context);

        void AddParameterStopwatchName();
        void AddOptionAdditionalText();
        void AddSwitchVerboseOutput(bool default_value);
        void AddSwitchQuiet(bool default_value);
        void AddSwitchIgnoreInvalidState(bool default_value);
        void AddSwitchShowElapsedTime(bool default_value);
        void AddOptionElapsedTimeDisplayFormatStillActive();
        void AddOptionElapsedTimeDisplayFormatNotActive();

    public:
        string GetDataFileName();

        string GetStopwatchName();
        bool GetVerboseOutput();
        bool GetIgnoreInvalidState();
        bool GetShowElapsedTime();
        string GetElapsedTimeDisplayFormat();
        string GetArgumentAdditionalText();
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

        static string GetDataFileDetails(const TimerRepository& repository)
        {
            return string("Accessing Data File: ")
                .append(repository.GetFileName());
        }

        static void ShowDataFileDetails(const TimerRepository& repository)
        {
            cout << GetDataFileDetails(repository) << endl
                << endl;
        }

        static string GetTimerDisplayDetailsText(const Timer& timer)
        {
            const TimerStateTypeTextResolver resolver;

            return timer.GetName()
                .append(" ")
                .append(TimerDisplayBuilder::GetFormattedStartTime(timer.GetStartDateTime(), TimerDisplayBuilder::DefaultStartTimeTextFormat))
                .append(" - ")
                .append(resolver.GetText(timer.GetState()));
        }

        static void ShowTimerDisplayDetails(const Timer& timer)
        {
            cout << GetTimerDisplayDetailsText(timer) << endl
                << endl;;
        }

        static void ShowTimerDetailsElapsed(const Timer& timer, BaseArguments& arguments, const string& status)
        {
            const auto text = TimerDisplayBuilder::GetFormattedDisplayText(timer, arguments.GetElapsedTimeDisplayFormat(), status);
            cout << text << endl;
        }

        static void ShowFormattedElapsedTime(const Timer& timer, const string& display_format, const string& additional_text)
        {
            string text = TimerDisplayBuilder::GetFormattedDisplayText(timer, display_format);
            if (!additional_text.empty())
                text = text.append(" - ").append(additional_text);
            cout << text << endl;
        }

    public:
        [[nodiscard]] CommandType GetCommandType() const { return m_command_type; }
    };
}
