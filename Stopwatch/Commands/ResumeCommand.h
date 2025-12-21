#pragma once

#include "BaseCommand.h"
#include "../stdafx.h"
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizeUseEqualsDefault
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppTooWideScopeInitStatement

using namespace std;

namespace Stopwatch
{
    class ResumeArguments final : public BaseArguments
    {
        static const ParserContext m_parser_context;

    public:
        ResumeArguments()
            : BaseArguments(m_parser_context)
        {
            AddParameterStopwatchName();
            AddOptionAdditionalText();
            AddSwitchShowElapsedTime(false);
            AddOptionElapsedTimeDisplayFormat();
            AddSwitchIgnoreInvalidState(false);
            AddSwitchVerboseOutput(true);
        }
    };

    //------------------------------------------------------------------------------

    class ResumeCommand final : public BaseCommand
    {
        ResumeArguments m_arguments;

    public:
        ResumeCommand()
            : BaseCommand(&m_arguments, CommandType::RESUME, "Resume a paused Stopwatch", 40)
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

            if (!timer.CanStart())
            {
                if (!m_arguments.GetIgnoreInvalidState())
                    AbortInvalidState(timer, CommandType::RESUME);
            }

            timer.Start();

            if (m_arguments.GetShowElapsedTime())
                ShowTimerDetailsElapsed(timer, m_arguments, "stopped");

            if (m_arguments.GetShowElapsedTime())
                ShowFormattedElapsedTime(timer, m_arguments.GetElapsedTimeDisplayFormat(), m_arguments.GetArgumentAdditionalText());

            repository.Update(timer);
        }
    };
}
