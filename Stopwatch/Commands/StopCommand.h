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
            AddParameterStopwatchName();
            AddSwitchVerboseOutput(true);
        }
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

            auto timer = repository.GetByName(stopwatch_name);
            if (timer.IsEmpty())
                AbortNotFound(stopwatch_name);

            timer.Stop();

            if (m_arguments.GetVerbose())
                cout << GetElapsedTimeDisplay(timer, "Stopped") << endl;

            repository.Delete(timer);
        }
    };
}
