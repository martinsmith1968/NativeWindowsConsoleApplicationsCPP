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
// ReSharper disable CppTooWideScopeInitStatement

using namespace std;

namespace Stopwatch
{
    class ElapsedArguments final : public BaseArguments
    {
    public:
        ElapsedArguments()
            : BaseArguments(ParserContext(StringUtils::ToLower(CommandTypeTextResolver().GetText(CommandType::ELAPSED))))
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

            const auto& timer = repository.GetByName(stopwatch_name);
            if (timer.IsEmpty())
                AbortNotFound(stopwatch_name);

            string text = TimerDisplayBuilder::GetFormattedText(timer, m_arguments.GetElapsedTimeDisplayFormat());
            const string additional_text = m_arguments.GetArgumentAdditionalText();
            if (!additional_text.empty())
                text = text.append(" - ").append(additional_text);
            cout << text << endl;
        }
    };
}
