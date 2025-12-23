#pragma once

#include "BaseCommand.h"
#include "../stdafx.h"
#include <iostream>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizeUseEqualsDefault
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers

using namespace std;

namespace Stopwatch
{
    class StartArguments final : public BaseArguments
    {
        const string ArgumentNameForce = "force";

        static const ParserContext m_parser_context;

    public:
        StartArguments()
            : BaseArguments(m_parser_context)
        {
            AddParameterStopwatchName();
            AddSwitchVerboseOutput(false);
            AddSwitchQuiet(false);
            AddSwitch("f", ArgumentNameForce, false, "Force starting even if already exists", false);
        }

        bool GetForce() { return GetSwitchValue(ArgumentNameForce); }
        bool GetQuiet() { return GetSwitchValue(ArgumentNameQuiet); }
    };

    //------------------------------------------------------------------------------

    class StartCommand final : public BaseCommand
    {
        const string START_TIME_DISPLAY_FORMAT = "%H:%M:%S";

        StartArguments m_arguments;

    public:
        StartCommand()
            : BaseCommand(&m_arguments, CommandType::START, "Start a named Stopwatch", 20)
        {
        }

        void Execute() override
        {
            const auto stopwatch_name = m_arguments.GetStopwatchName();
            auto repository = TimerRepository(m_arguments.GetDataFileName());

            if (m_arguments.GetVerboseOutput())
                ShowDataFileDetails(repository);

            auto timer = repository.GetByName(stopwatch_name);
            if (!timer.IsEmpty())
            {
                if (!m_arguments.GetForce())
                    AbortAlreadyExists(stopwatch_name);

                repository.Delete(timer);
            }

            timer = Timer(stopwatch_name);
            timer.Start();
            repository.Add(timer);

            if (!m_arguments.GetQuiet())
            {
                cout << timer.GetName()
                        << " started at " + TimerDisplayBuilder::GetFormattedStartTime(timer.GetStartDateTime(), START_TIME_DISPLAY_FORMAT)
                        << endl;
            }
        }
    };
}
