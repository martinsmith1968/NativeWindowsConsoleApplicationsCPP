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
    class StopArguments final : public BaseArguments
    {
        const ParserContext m_parser_context = ParserContext(StringUtils::ToLower(CommandTypeTextResolver().GetText(CommandType::STOP)));

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

            auto timer = repository.GetByName(stopwatch_name);
            if (timer.IsEmpty())
                AbortNotFound(stopwatch_name);

            timer.Stop();

            if (m_arguments.GetVerboseOutput())
                cout << GetTimerStatusDisplayText(timer, "stopped") << endl;

            if (m_arguments.GetShowElapsedTime())
            {
                const auto text = TimerDisplayBuilder::GetFormattedText(timer, m_arguments.GetElapsedTimeDisplayFormat(), "stopped");
                cout << text << endl;
            }

            repository.Delete(timer);
        }
    };
}
