#pragma once

#include "../stdafx.h"
#include "BaseCommand.h"
#include <string>
#include <iostream>

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
        const ParserContext m_parser_context = ParserContext(StringUtils::ToLower(CommandTypeTextResolver().GetText(CommandType::RESUME)));

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

            auto timer = repository.GetByName(stopwatch_name);
            if (timer.IsEmpty())
                AbortNotFound(stopwatch_name);

            if (!timer.CanStart())
            {
                if (!m_arguments.GetIgnoreInvalidState())
                    AbortInvalidState(timer, CommandType::RESUME);
            }

            timer.Start();

            if (m_arguments.GetVerboseOutput())
                cout << GetTimerStatusDisplayText(timer, "resumed") << endl;

            if (m_arguments.GetShowElapsedTime())
            {
                string text = TimerDisplayBuilder::GetFormattedText(timer, m_arguments.GetElapsedTimeDisplayFormat());
                const string additional_text = m_arguments.GetArgumentAdditionalText();
                if (!additional_text.empty())
                    text = text.append(" - ").append(additional_text);
                cout << text << endl;
            }

            repository.Update(timer);
        }
    };
}
