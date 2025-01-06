#pragma once

#include "../stdafx.h"
#include "BaseCommand.h"
#include "../../DNX.Utils/MapUtils.h"
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
    class ResumeArguments final : public BaseArguments
    {

    public:
        ResumeArguments()
        {
            AddParameter(ValueType::STRING, 1, ArgumentNameStopwatchName, "", "The name of the Stopwatch", true);
            AddSwitch("v", ArgumentNameVerbose, StringUtils::BoolToString(true), "Control verbosity of output messages", false, 0);
        }

        string GetStopwatchName() { return GetArgumentValue(ArgumentNameStopwatchName); }
        bool GetVerbose() { return GetSwitchValue(ArgumentNameVerbose); }
    };

    class ResumeCommand final : public BaseCommand
    {
        ResumeArguments m_arguments;

    public:
        ResumeCommand()
            : BaseCommand(&m_arguments, CommandType::RESUME, "Resume a paused Stopwatch", 40)
        { }

        void Execute() override
        {
            const auto stopwatch_name = m_arguments.GetStopwatchName();

            auto repository = TimerRepository(m_arguments.GetFileName());

            auto timers = repository.ReadAll();
            if (!MapUtils::Exists(timers, stopwatch_name)) // timers.contains(stopwatch_name) != timers.end())
            {
                const string exception_text = stopwatch_name + " not found";
                throw exception(exception_text.c_str());
            }

            auto timer = timers.at(stopwatch_name);
            if (timer.GetState() != TimerStateType::PAUSED)
            {
                const string exception_text = stopwatch_name + " is not currently paused";
                throw exception(exception_text.c_str());
            }

            timer.Start();

            if (m_arguments.GetVerbose())
            {
                const auto start_time = timer.GetStartDateTime();
                const auto formatted_start_time = FormatForDisplay(start_time);
                cout << "Started: " << stopwatch_name << " - " << formatted_start_time << endl;
            }

            repository.SaveAll(timers);
        }
    };
}
