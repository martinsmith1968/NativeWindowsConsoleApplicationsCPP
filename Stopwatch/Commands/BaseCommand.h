#pragma once
#include "../stdafx.h"
#include "../TimerRepository.h"
#include "../../DNX.App/Arguments.h"
#include "../../DNX.App/ValueType.h"
#include <chrono>
#include <string>

#include "CommandType.h"

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
        const string ArgumentNameFormat = "output-format";
        const string ArgumentNameVerbose = "verbose";
        const string ArgumentNameFileName = "filename";

        BaseArguments()
        {
            AddOption(ValueType::STRING, "f", ArgumentNameFileName, TimerRepository::GetDefaultRepositoryFileName(), "The filename to store Stopwatch data in", false, APP_MAX);
        }

    public:
        string GetFileName() { return GetArgumentValue(ArgumentNameFileName); }
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
            const string& description
        )
            : Command(arguments, CommandTypeTextResolver().GetText(commandType), description)
        {
            m_command_type = commandType;
        }

        void Execute() override = 0;

        static string FormatForDisplay(const tm& tm)
        {
            char buffer[20];

            auto size = strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", &tm);

            return buffer;
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
        CommandType GetCommandType() const { return m_command_type; }
    };
}
