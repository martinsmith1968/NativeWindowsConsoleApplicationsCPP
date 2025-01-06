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
    class PauseArguments final : public BaseArguments
    {

    public:
        PauseArguments()
        {
            AddParameterStopwatchName();
            AddSwitchVerboseOutput(true);
        }
    };

    class PauseCommand final : public BaseCommand
    {
        PauseArguments m_arguments;

    public:
        PauseCommand()
            : BaseCommand(&m_arguments, CommandType::PAUSE, "Pause an active Stopwatch", 40)
        {
        }

        void Execute() override
        {
            const auto stopwatch_name = m_arguments.GetStopwatchName();
            auto repository = TimerRepository(m_arguments.GetFileName());

            auto timer = repository.GetByName(stopwatch_name);
            if (timer.IsEmpty())
                AbortNotFound(stopwatch_name);

            timer.Stop();

            if (m_arguments.GetVerbose())
                cout << GetElapsedTimeDisplay(timer, "Paused") << endl;

            repository.Update(timer);
        }
    };
}
