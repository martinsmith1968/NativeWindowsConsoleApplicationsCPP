#pragma once

#include "../stdafx.h"
#include "BaseCommand.h"
#include "../../DNX.Utils/StringUtils.h"
#include <string>
#include <iostream>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizeUseEqualsDefault
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyPerformanceAvoidEndl

using namespace std;

namespace Stopwatch
{
    class CancelArguments final : public BaseArguments
    {
        const ParserContext m_parser_context = ParserContext(StringUtils::ToLower(CommandTypeTextResolver().GetText(CommandType::CANCEL)));

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

            auto timer = repository.GetByName(stopwatch_name);
            if (timer.IsEmpty())
                AbortNotFound(stopwatch_name);

            timer.Stop();

            if (m_arguments.GetVerboseOutput())
                cout << GetTimerStatusDisplayText(timer, "cancelled") << endl;

            repository.Delete(timer);
        }
    };
}
