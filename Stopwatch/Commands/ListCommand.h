#pragma once

#include "../stdafx.h"
#include "BaseCommand.h"
#include "../../DNX.Utils/StringUtils.h"
#include <string>
#include <chrono>
#include <iostream>
#include <ostream>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizeUseEqualsDefault
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace Stopwatch
{
    enum class ListFormatType : uint8_t
    {
        DISPLAY,
        CSV,
        CUSTOM
    };

    class ListFormatTypeText : public EnumTextResolver<ListFormatType>
    {
    public:
        ListFormatTypeText()
        {
            SetText(ListFormatType::DISPLAY, "Display");
            SetText(ListFormatType::CSV, "CSV");
            SetText(ListFormatType::CUSTOM, "Custom");
        }
    };

    class ListArguments final : public BaseArguments
    {
    public:
        ListArguments()
        {
            AddOption(ValueType::STRING, "o", ArgumentNameFormat, ListFormatTypeText().GetText(ListFormatType::DISPLAY), "Control output format of list", false, 0, ListFormatTypeText().GetAllText());
            AddSwitch("v", ArgumentNameVerbose, StringUtils::BoolToString(false), "Control verbosity of output messages", false, 0);
        }

        string GetFormat() { return GetArgumentValue(ArgumentNameFormat); }
        bool GetVerbose() { return GetSwitchValue(ArgumentNameVerbose); }
    };

    class ListCommand final : public BaseCommand
    {
        ListArguments m_arguments;

    public:
        ListCommand()
            : BaseCommand(&m_arguments, CommandType::LIST, "List all active Stopwatches")
        {
        }

        void Execute() override
        {
            const auto repository = TimerRepository(m_arguments.GetFileName());

            const auto timers = repository.ReadAll();

            for (const auto& [key, value] : timers)
            {
                cout << value.GetName() << "  " << FormatForDisplay(value.GetStartDateTime()) << " - " << FormatForDisplay(value.GetAccumulatedElapsed()) << endl;
            }
        }
    };
}
