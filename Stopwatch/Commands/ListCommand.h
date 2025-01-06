#pragma once

#include "../stdafx.h"
#include "BaseCommand.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <ostream>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizeUseEqualsDefault
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions
// ReSharper disable CppClangTidyClangDiagnosticCoveredSwitchDefault

using namespace std;

namespace Stopwatch
{
    enum class OutputFormatType : uint8_t
    {
        DISPLAY,
        CSV,
        CUSTOM
    };

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

    class ListArguments final : public BaseArguments
    {
        const string ArgumentNameOutputFormat     = "output-format";
        const string ArgumentNameCustomFormatText = "custom-format-text";

    public:
        ListArguments()
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

    class BaseOutputFormatBuilder
    {
    public:
        virtual ~BaseOutputFormatBuilder() = default;
        virtual void Reset(ListArguments& arguments) {}
        virtual void PreProcess(const Timer& timer) {}
        [[nodiscard]] virtual string GetOutputText(const Timer& timer) const = 0;
    };

    class DisplayOutputFormatBuilder final : public BaseOutputFormatBuilder
    {
        size_t m_max_name_width = 0;

    public:
        void Reset(ListArguments& arguments) override
        {
            m_max_name_width = 0;
        }
        void PreProcess(const Timer& timer) override
        {
            m_max_name_width = max(m_max_name_width, timer.GetName().length());
        }
        [[nodiscard]] string GetOutputText(const Timer& timer) const override
        {
            stringstream ss;

            ss << left
                << setw(static_cast<streamsize>(m_max_name_width))
                << timer.GetName()
                << " Started: "
                << BaseCommand::FormatForDisplay(timer.GetStartDateTime())
                << " ("
                << TimerStateTypeTextResolver().GetText(timer.GetState())
                << ")"
                << " Elapsed: "
                << BaseCommand::FormatForDisplay(timer.GetAccumulatedElapsed())
                ;

            return ss.str();
        }
    };

    class CSVOutputFormatBuilder final : public BaseOutputFormatBuilder
    {
    public:
        [[nodiscard]] string GetOutputText(const Timer& timer) const override
        {
            stringstream ss;

            ss << timer.GetName()
                << ","
                << BaseCommand::FormatForDisplay(timer.GetStartDateTime())
                << ","
                << TimerStateTypeTextResolver().GetText(timer.GetState())
                << ","
                << timer.GetAccumulatedElapsed()
                ;

            return ss.str();
        }
    };

    class CustomOutputFormatBuilder final : public BaseOutputFormatBuilder
    {
        string m_custom_format_string;

    public:
        void Reset(ListArguments& arguments)
        {
            m_custom_format_string = arguments.GetCustomFormatText();
        }

        static string ApplyValuesToFormat(const string& custom_format, const Timer& timer)
        {
            auto start_datetime = timer.GetStartDateTime();

            auto text = custom_format;
            text = StringUtils::ReplaceString(text, "#NAME#", timer.GetName());
            text = StringUtils::ReplaceString(text, "#STATE#", TimerStateTypeTextResolver().GetText(timer.GetState()));
            text = StringUtils::ReplaceString(text, "#ACCUMULATED_ELAPSED#", to_string(timer.GetAccumulatedElapsed()));
            text = StringUtils::ReplaceString(text, "#TOTAL_ELAPSED#", to_string(timer.GetTotalElapsed()));
            text = StringUtils::ReplaceString(text, "#CURRENT_ELAPSED#", to_string(timer.GetCurrentElapsed()));
            text = StringUtils::ReplaceString(text, "#ELAPSED_TEXT#", BaseCommand::FormatForDisplay(timer.GetAccumulatedElapsed()));
            text = DateUtils::FormatDate(&start_datetime, text);

            return text;
        }

        [[nodiscard]] string GetOutputText(const Timer& timer) const override
        {
            auto text = ApplyValuesToFormat(m_custom_format_string, timer);

            return text;
        }
    };

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
            const auto repository = TimerRepository(m_arguments.GetFileName());

            const auto timers = repository.ReadAll();

            const auto builder = OutputFormatFactory().GetOutputFormatBuilder(m_arguments.GetOutputFormatType());

            builder->Reset(m_arguments);

            for (const auto& [key, value] : timers)
                builder->PreProcess(value);

            for (const auto& [key, value] : timers)
            {
                cout << builder->GetOutputText(value) << endl;
            }
        }
    };
}
