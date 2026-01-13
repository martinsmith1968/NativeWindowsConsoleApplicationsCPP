#pragma once

#include "BaseCommand.h"
#include "../stdafx.h"
#include "../../../libs/DNX.Utils/DateTime.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizeUseEqualsDefault
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers

using namespace std;

namespace Stopwatch
{
    class PurgeArguments final : public BaseArguments
    {
        const string ArgumentNameAgeDays    = "days";
        const string ArgumentNameAgeHours   = "hours";
        const string ArgumentNameTimerState = "state";

        static const ParserContext m_parser_context;

    public:
        PurgeArguments()
            : BaseArguments(m_parser_context)
        {
            AddOption(ValueType::INT, "d", ArgumentNameAgeDays, "", "Remove Timers started before specified days", false);
            AddOption(ValueType::INT, "h", ArgumentNameAgeHours, "", "Remove Timers started before specified hours", false);
            AddOption(ValueType::STRING, "s", ArgumentNameTimerState, "", "Limit which Timer states can be removed", false, 0, TimerStateTypeTextResolver().GetAllText());
            AddSwitchVerboseOutput(true);
        }

        void PostParseValidate() override
        {
            if (HasAgeDays() && GetAgeDays() < 0)
            {
                AddError(ArgumentNameAgeDays + " must be 0 or greater");
            }

            if (HasAgeHours() && GetAgeHours() < 0)
            {
                AddError(ArgumentNameAgeHours + " must be 0 or greater");
            }

            if (HasAgeDays() && HasAgeHours())
            {
                AddError("Only one of " + ArgumentNameAgeDays + " or " + ArgumentNameAgeHours + " can be specified");
            }

            if (!(HasAgeDays() || HasAgeHours()))
            {
                AddError("One of " + ArgumentNameAgeDays + " or " + ArgumentNameAgeHours + " must be specified");
            }
        }

        bool HasTimerStateType() { return HasArgumentValue(ArgumentNameTimerState); }
        bool HasAgeDays() { return HasArgumentValue(ArgumentNameAgeDays); }
        bool HasAgeHours() { return !GetArgumentValue(ArgumentNameAgeHours).empty(); }

        TimerStateType GetTimerStateType() { return TimerStateTypeTextResolver().GetValue(GetArgumentValue(ArgumentNameTimerState)); }
        int GetAgeDays() { return ValueConverter::ToInt(GetArgumentValue(ArgumentNameAgeDays)); }
        int GetAgeHours() { return ValueConverter::ToInt(GetArgumentValue(ArgumentNameAgeHours)); }
    };

    //------------------------------------------------------------------------------

    class PurgeCommand final : public BaseCommand
    {
        PurgeArguments m_arguments;

    public:
        PurgeCommand()
            : BaseCommand(&m_arguments, CommandType::PURGE, "Purge old Stopwatches", 40)
        {
        }

        void Execute() override
        {
            auto repository = TimerRepository(m_arguments.GetDataFileName());

            if (m_arguments.GetVerboseOutput())
                ShowDataFileDetails(repository);

            const auto now = DateTime::Now();
            const auto max_start_date = m_arguments.HasAgeDays()
                ? now.GetDate().AddDays(m_arguments.GetAgeDays() * -1)
                : m_arguments.HasAgeHours()
                    ? now.GetDate().AddHours(now.GetHour()).AddMinutes(now.GetMinute()).AddSeconds(now.GetSeconds())
                        .AddHours(m_arguments.GetAgeHours() * -1)
                    : DateTime::Min();

            if (m_arguments.GetVerboseOutput())
            {
                cout << "Purging all Timers started before: "
                    << max_start_date.ToString(max_start_date.IsDateOnly() ? DateTime::Formats::Date_Display : DateTime::Formats::Display)
                    << (m_arguments.HasTimerStateType() ? " with State: " + TimerStateTypeTextResolver().GetText(m_arguments.GetTimerStateType()) : "")
                    << endl;
            }

            const auto all_timers = repository.ReadAll();
            const auto timers = MapUtils::GetValues(all_timers);

            if (m_arguments.GetVerboseOutput())
            {
                cout << endl
                    << "Total Timers found: " << timers.size() << endl;
            }

            auto deleted_count = 0;
            auto line_output_started = false;
            for (const auto& timer : timers)
            {
                if (timer.GetStart() < max_start_date.GetAsTimeT())
                {
                    if (m_arguments.HasTimerStateType())
                    {
                        if (timer.GetState() != m_arguments.GetTimerStateType())
                        {
                            if (m_arguments.GetVerboseOutput())
                            {
                                if (!line_output_started)
                                {
                                    cout << endl;
                                    line_output_started = true;
                                }

                                cout << "Ignoring Timer:" << GetTimerDisplayDetailsText(timer)
                                    << " - State does not match: " << TimerStateTypeTextResolver().GetText(timer.GetState())
                                    << endl;
                            }
                            continue;
                        }
                    }

                    if (!line_output_started)
                    {
                        cout << endl;
                        line_output_started = true;
                    }
                    cout << "Deleting Timer: " << GetTimerDisplayDetailsText(timer) << endl;
                    ++deleted_count;

                    repository.Delete(timer);
                }
                else
                {
                    if (m_arguments.GetVerboseOutput())
                    {
                        if (!line_output_started)
                        {
                            cout << endl;
                            line_output_started = true;
                        }

                        cout << "Ignoring Timer: " << GetTimerDisplayDetailsText(timer)
                            << " - Start Date is after threshold: " << DateTime(timer.GetStart()).ToString(DateTime::Formats::Sortable)
                            << endl;
                    }
                }
            }

            cout << endl
                << "Timers deleted: " << deleted_count << endl;
        }
    };
}
