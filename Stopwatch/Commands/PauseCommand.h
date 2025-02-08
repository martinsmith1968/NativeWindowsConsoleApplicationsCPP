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
    class PauseArguments final : public BaseArguments
    {

    public:
        PauseArguments()
        {
            AddParameterStopwatchName();
            AddOptionAdditionalText();
            AddSwitchShowElapsedTime(true);
            AddOptionElapsedTimeAlternativeDisplayFormat();
            AddSwitchIgnoreInvalidState(false);
            AddSwitchVerboseOutput(false);
        }
    };

    class PauseCommand final : public BaseCommand
    {
        PauseArguments m_arguments;

    public:
        PauseCommand()
            : BaseCommand(&m_arguments, CommandType::PAUSE, "Pause an active Stopwatch", 40)
        { }

        void Execute() override
        {
            const auto stopwatch_name = m_arguments.GetStopwatchName();
            auto repository = TimerRepository(m_arguments.GetDataFileName());

            auto timer = repository.GetByName(stopwatch_name);
            if (timer.IsEmpty())
                AbortNotFound(stopwatch_name);

            if (!timer.CanStop())
            {
                if (!m_arguments.GetIgnoreInvalidState())
                    AbortInvalidState(timer, CommandType::STOP);
            }

            timer.Pause();

            if (m_arguments.GetVerboseOutput())
                cout << GetTimerStatusDisplayText(timer, "paused") << endl;

            if (m_arguments.GetShowElapsedTime())
            {
                string text = TimerDisplayBuilder::GetFormattedText(timer, m_arguments.GetElapsedTimeDisplayFormat(), "paused");
                const string additional_text = m_arguments.GetArgumentAdditionalText();
                if (!additional_text.empty())
                    text = text.append(" - ").append(additional_text);
                cout << text << endl;
            }

            repository.Update(timer);
        }
    };
}
