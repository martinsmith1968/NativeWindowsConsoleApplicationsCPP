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
    class ElapsedArguments final : public BaseArguments
    {
        static const ParserContext m_parser_context;

    public:
        ElapsedArguments()
            : BaseArguments(m_parser_context)
        {
            AddParameterStopwatchName();
            AddOptionAdditionalText();
            AddOptionElapsedTimeDisplayFormat();
        }
    };

    //------------------------------------------------------------------------------

    class ElapsedCommand final : public BaseCommand
    {
        ElapsedArguments m_arguments;

    public:
        ElapsedCommand()
            : BaseCommand(&m_arguments, CommandType::ELAPSED, "Display the current elapsed time of an active Stopwatch", 30)
        {
        }

        void Execute() override
        {
            const auto stopwatch_name = m_arguments.GetStopwatchName();
            const auto repository = TimerRepository(m_arguments.GetDataFileName());

            if (m_arguments.GetVerboseOutput())
                ShowDataFileDetails(repository);

            const auto& timer = repository.GetByName(stopwatch_name);
            if (timer.IsEmpty())
                AbortNotFound(stopwatch_name);

            if (m_arguments.GetVerboseOutput())
                ShowTimerDisplayDetails(timer);

            ShowFormattedElapsedTime(timer, m_arguments.GetElapsedTimeDisplayFormat(), m_arguments.GetArgumentAdditionalText());
        }
    };
}
