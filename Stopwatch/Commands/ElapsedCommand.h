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
    class ElapsedArguments final : public BaseArguments
    {

    public:
        ElapsedArguments()
        {
            AddParameterStopwatchName();
            AddOptionElapsedTimeDisplayFormat();
        }
    };

    class ElapsedCommand final : public BaseCommand
    {
        ElapsedArguments m_arguments;

    public:
        ElapsedCommand()
            : BaseCommand(&m_arguments, CommandType::ELAPSED, "Display the current elapsed time of an active Stopwatch", 30)
        { }

        void Execute() override
        {
            const auto stopwatch_name = m_arguments.GetStopwatchName();
            const auto repository = TimerRepository(m_arguments.GetFileName());

            const auto& timer = repository.GetByName(stopwatch_name);
            if (timer.IsEmpty())
                AbortNotFound(stopwatch_name);

            const auto text = TimerDisplayBuilder::GetFormattedText(timer, m_arguments.GetElapsedTimeDisplayFormat());
            cout << text << endl;
        }
    };
}
