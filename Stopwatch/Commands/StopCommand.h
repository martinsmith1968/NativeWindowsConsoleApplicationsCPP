#pragma once

#include "BaseCommand.h"
#include "../stdafx.h"
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizeUseEqualsDefault
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyPerformanceAvoidEndl

using namespace std;

namespace Stopwatch
{
    class StopArguments final : public BaseArguments
    {
        static const ParserContext m_parser_context;

    public:
        StopArguments()
            : BaseArguments(m_parser_context)
        {
            AddParameterStopwatchName();
            AddSwitchShowElapsedTime(true);
            AddOptionElapsedTimeAlternativeDisplayFormat();
            AddSwitchVerboseOutput(false);
        }
    };

    //------------------------------------------------------------------------------

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
            auto repository = TimerRepository(m_arguments.GetDataFileName());

            if (m_arguments.GetVerboseOutput())
                ShowDataFileDetails(repository);

            auto timer = repository.GetByName(stopwatch_name);
            if (timer.IsEmpty())
                AbortNotFound(stopwatch_name);

            if (m_arguments.GetVerboseOutput())
                ShowTimerDisplayDetails(timer);

            timer.Stop();

            if (m_arguments.GetShowElapsedTime())
                ShowTimerDetailsElapsed(timer, m_arguments, "stopped");

            repository.Delete(timer);
        }
    };
}
