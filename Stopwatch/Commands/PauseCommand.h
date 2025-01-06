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
    class PauseArguments final : public BaseArguments
    {

    public:
        PauseArguments()
        {
            AddParameter(ValueType::STRING, 1, ArgumentNameStopwatchName, "", "The name of the Stopwatch", true);
            AddSwitch("v", ArgumentNameVerbose, StringUtils::BoolToString(true), "Control verbosity of output messages", false, 0);
        }

        string GetStopwatchName() { return GetArgumentValue(ArgumentNameStopwatchName); }
        bool GetVerbose() { return GetSwitchValue(ArgumentNameVerbose); }
    };

    class PauseCommand final : public BaseCommand
    {
        PauseArguments m_arguments;

    public:
        PauseCommand()
            : BaseCommand(&m_arguments, CommandType::PAUSE, "Pause an active Stopwatch")
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

            auto timer = timers.at(stopwatch_name);
            if (timer.GetState() != TimerStateType::RUNNING)
            {
                const string exception_text = stopwatch_name + " is not currently active";
                throw exception(exception_text.c_str());
            }

            timer.Stop();

            if (m_arguments.GetVerbose())
            {

                const auto start_time = timer.GetStartDateTime();
                const auto formatted_start_time = FormatForDisplay(start_time);
                cout << timer.GetName() << "  " << FormatForDisplay(timer.GetStartDateTime()) << " - " << FormatForDisplay(timer.GetAccumulatedElapsed()) << endl;
            }

            repository.SaveAll(timers);
        }
    };
}
