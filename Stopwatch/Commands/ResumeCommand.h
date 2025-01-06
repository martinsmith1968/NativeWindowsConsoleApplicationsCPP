#pragma once

#include "../stdafx.h"
#include "BaseCommand.h"
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
            AddParameterStopwatchName();
            AddSwitchVerboseOutput(true);
        }
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

            auto timer = repository.GetByName(stopwatch_name);
            if (timer.IsEmpty())
                AbortNotFound(stopwatch_name);

            timer.Start();

            if (m_arguments.GetVerbose())
                cout << GetTimerDetailsDisplay(timer, "Resumed") << endl;

            repository.Update(timer);
        }
    };
}
