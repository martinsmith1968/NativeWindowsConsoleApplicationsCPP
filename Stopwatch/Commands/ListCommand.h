#pragma once

#include "BaseCommand.h"
#include "../stdafx.h"
#include "../../DNX.Utils/MapUtils.h"
#include "../../DNX.Utils/StringUtils.h"
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizeUseEqualsDefault
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions
// ReSharper disable CppClangTidyClangDiagnosticCoveredSwitchDefault
// ReSharper disable CppClangTidyClangDiagnosticPadded

using namespace std;

namespace Stopwatch
{
    enum class OutputFormatType : uint8_t
    {
        DISPLAY,
        CSV,
        CUSTOM
    };

    //------------------------------------------------------------------------------

    class OutputFormatTypeTextResolver : public EnumTextResolver<OutputFormatType>
    {
    public:
        OutputFormatTypeTextResolver()
        {
            SetText(OutputFormatType::DISPLAY, "Display");
            SetText(OutputFormatType::CSV, "CSV");
            SetText(OutputFormatType::CUSTOM, "Custom");
        }
    };

    //------------------------------------------------------------------------------

    class ListArguments final : public BaseArguments
    {
        const string ArgumentNameOutputFormat     = "output-format";
        const string ArgumentNameCustomFormatText = "custom-format-text";

        static const ParserContext m_parser_context;

    public:
        ListArguments()
            : BaseArguments(m_parser_context)
        {
            AddOption(ValueType::STRING, "o", ArgumentNameOutputFormat, OutputFormatTypeTextResolver().GetText(OutputFormatType::DISPLAY), "Control output format of list", false, 0, OutputFormatTypeTextResolver().GetAllText());
            AddOption(ValueType::STRING, "fmt", ArgumentNameCustomFormatText, "", "A custom format string for the Timer details", false);
            AddSwitchVerboseOutput(false);
        }

        void PostParseValidate() override
        {
            if (GetOutputFormatType() == OutputFormatType::CUSTOM)
            {
                if (StringUtils::Trim(GetCustomFormatText()).empty())
                {
                    AddError(ArgumentNameCustomFormatText + " is required for Output Format: " + OutputFormatTypeTextResolver().GetText(GetOutputFormatType()));
                }
            }
        }

        OutputFormatType GetOutputFormatType() { return OutputFormatTypeTextResolver().GetValue(GetArgumentValue(ArgumentNameOutputFormat)); }
        string GetCustomFormatText() { return GetArgumentValue(ArgumentNameCustomFormatText); }
    };

    //------------------------------------------------------------------------------

    class BaseOutputFormatBuilder
    {
    public:
        virtual ~BaseOutputFormatBuilder() = default;
        virtual void Reset(ListArguments& arguments) {}
        virtual void PreProcess(const Timer& timer) {}
        [[nodiscard]] virtual string GetOutputText(const Timer& timer) const = 0;
    };

    //------------------------------------------------------------------------------

    class DisplayOutputFormatBuilder final : public BaseOutputFormatBuilder
    {
        size_t m_max_name_width  = 0;
        size_t m_max_state_width = 0;

    public:
        void Reset(ListArguments& arguments) override
        {
            m_max_name_width  = 0;
            m_max_state_width = 0;
        }
        void PreProcess(const Timer& timer) override
        {
            m_max_name_width  = max(m_max_name_width, timer.GetName().length());
            m_max_state_width = max(m_max_state_width, TimerStateTypeTextResolver().GetText(timer.GetState()).length());
        }
        [[nodiscard]] string GetOutputText(const Timer& timer) const override
        {
            stringstream ss;

            ss << left
                << setw(static_cast<streamsize>(m_max_name_width))
                << timer.GetName()
                << ": "
                << setw(static_cast<streamsize>(m_max_state_width))
                << TimerStateTypeTextResolver().GetText(timer.GetState())
                << " - "
                << TimerDisplayBuilder::GetFormattedDisplayText(timer, TimerDisplayBuilder::DefaultElapsedTimeTextFormat)
                ;

            return ss.str();
        }
    };

    //------------------------------------------------------------------------------

    class CSVOutputFormatBuilder final : public BaseOutputFormatBuilder
    {
    public:
        [[nodiscard]] string GetOutputText(const Timer& timer) const override
        {
            stringstream ss;

            ss << timer.GetName()
                << ","
                << TimerDisplayBuilder::GetFormattedStartTime(timer.GetStartDateTime(), TimerDisplayBuilder::DefaultStartTimeTextFormat)
                << ","
                << TimerStateTypeTextResolver().GetText(timer.GetState())
                << ","
                << TimerDisplayBuilder::GetFormattedElapsedTime(timer.GetAccumulatedElapsed(), TimerDisplayBuilder::DefaultElapsedTimeTextFormat)
                ;

            return ss.str();
        }
    };

    //------------------------------------------------------------------------------

    class CustomOutputFormatBuilder final : public BaseOutputFormatBuilder
    {
        string m_custom_format_string;

    public:
        void Reset(ListArguments& arguments) override
        {
            m_custom_format_string = arguments.GetCustomFormatText();
        }

        [[nodiscard]] string GetOutputText(const Timer& timer) const override
        {
            return TimerDisplayBuilder::GetFormattedDisplayText(timer, m_custom_format_string);
        }
    };

    //------------------------------------------------------------------------------

    class OutputFormatFactory
    {
        DisplayOutputFormatBuilder _display_output_format_builder;
        CSVOutputFormatBuilder _csv_output_format_builder;
        CustomOutputFormatBuilder _custom_output_format_builder;

    public:
        BaseOutputFormatBuilder* GetOutputFormatBuilder(const OutputFormatType list_format_type)
        {
            switch (list_format_type)
            {
            case OutputFormatType::DISPLAY: return &_display_output_format_builder;
            case OutputFormatType::CSV: return &_csv_output_format_builder;
            case OutputFormatType::CUSTOM: return &_custom_output_format_builder;
            default: return nullptr;
            }
        }
    };

    //------------------------------------------------------------------------------

    class ListCommand final : public BaseCommand
    {
        ListArguments m_arguments;

    public:
        ListCommand()
            : BaseCommand(&m_arguments, CommandType::LIST, "List all active Stopwatches", 10)
        {
        }

        void Execute() override
        {
            const auto repository = TimerRepository(m_arguments.GetDataFileName());

            const auto all_timers = repository.ReadAll();
            auto timers = MapUtils::GetValues(all_timers);
            timers.sort(Timer::CompareByStartTime);

            const auto builder = OutputFormatFactory().GetOutputFormatBuilder(m_arguments.GetOutputFormatType());

            builder->Reset(m_arguments);

            for (const auto& timer : timers)
                builder->PreProcess(timer);

            for (const auto& timer : timers)
            {
                cout << builder->GetOutputText(timer) << endl;
            }
            if (!timers.empty())
                cout << endl;
            cout << "Found: " << timers.size() << " stopwatches" << endl;

            if (m_arguments.GetVerboseOutput())
            {
                cout << endl;
                cout << "Data File : " << m_arguments.GetDataFileName() << endl;
            }
        }
    };
}
