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

    class StopCommand final : public BaseCommand
    {
        StopArguments m_arguments;

    public:
        StopCommand()
            : BaseCommand(&m_arguments, CommandType::STOP, "Start an active Stopwatch", 20)
        {
        }

        void Execute() override
        {
            const auto stopwatch_name = m_arguments.GetStopwatchName();

            auto repository = TimerRepository(m_arguments.GetFileName());

            auto timers = repository.ReadAll();
            if (timers.find(stopwatch_name) == timers.end())
            {
                const string exception_text = stopwatch_name + " not found";
                throw exception(exception_text.c_str());
            }

            auto& timer = timers.at(stopwatch_name);
            timer.Stop();

            if (m_arguments.GetVerbose())
            {

                const auto start_time = timer.GetStartDateTime();
                const auto formatted_start_time = FormatForDisplay(start_time);
                cout << timer.GetName() << " - Total time: " << FormatForDisplay(timer.GetAccumulatedElapsed()) << endl;
            }

            timers.erase(timers.find(stopwatch_name));

            repository.SaveAll(timers);
        }
    };
}
