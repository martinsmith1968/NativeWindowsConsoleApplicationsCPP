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
    class CancelArguments final : public BaseArguments
    {
        static const ParserContext m_parser_context;

    public:
        CancelArguments()
            : BaseArguments(m_parser_context)
        {
            AddParameterStopwatchName();
            AddSwitchVerboseOutput(false);
        }
    };

    //------------------------------------------------------------------------------

    class CancelCommand final : public BaseCommand
    {
        CancelArguments m_arguments;

    public:
        CancelCommand()
            : BaseCommand(&m_arguments, CommandType::CANCEL, "Cancel an active Stopwatch", 25)
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

            repository.Delete(timer);
        }
    };
}
