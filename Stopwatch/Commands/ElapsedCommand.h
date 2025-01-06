#pragma once

#include "../stdafx.h"
#include "BaseCommand.h"
#include "../../DNX.Utils/StringUtils.h"
#include <string>
#include <iostream>
#include <ostream>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizeUseEqualsDefault
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyPerformanceAvoidEndl

using namespace std;

namespace Stopwatch
{
    class ElapsedArguments final : public BaseArguments
    {

    public:
        ElapsedArguments()
        {
            AddParameter(ValueType::STRING, 1, ArgumentNameStopwatchName, "", "The name of the Stopwatch", true);
            AddSwitch("v", ArgumentNameVerbose, StringUtils::BoolToString(true), "Control verbosity of output messages", false, 0);
        }

        string GetStopwatchName() { return GetArgumentValue(ArgumentNameStopwatchName); }
        bool GetVerbose() { return GetSwitchValue(ArgumentNameVerbose); }
    };

    class ElapsedCommand final : public BaseCommand
    {
        ElapsedArguments m_arguments;

    public:
        ElapsedCommand()
            : BaseCommand(&m_arguments, CommandType::ELAPSED, "Display the current elapsed time of an active Stopwatch")
        {
        }

        void Execute() override
        {
            const auto stopwatch_name = m_arguments.GetStopwatchName();

            const auto repository = TimerRepository(m_arguments.GetFileName());

            auto timers = repository.ReadAll();
            if (timers.find(stopwatch_name) == timers.end())
            {
                const string exception_text = stopwatch_name + " not found";
                throw exception(exception_text.c_str());
            }

            const auto timer = timers.at(stopwatch_name);

            const auto start_time = timer.GetStartDateTime();
            const auto formatted_start_time = FormatForDisplay(start_time);
            cout << timer.GetName() << " - Elapsed time: " << FormatForDisplay(timer.GetAccumulatedElapsed()) << endl;
        }
    };
}
