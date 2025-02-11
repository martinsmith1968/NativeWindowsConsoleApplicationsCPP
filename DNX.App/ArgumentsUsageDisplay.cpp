#include "stdafx.h"
#include "ArgumentsUsageDisplay.h"
#include "../DNX.Utils/ConsoleUtils.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/StringUtils.h"
#include <iomanip>
#include <iostream>
#include <ostream>

#include "ParserConfig.h"
#include "../DNX.Utils/ListUtils.h"

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;

string ArgumentsUsageDisplay::ErrorLinePrefix = "ERROR";

void ArgumentsUsageDisplay::ShowUsage(const Arguments& arguments, const ParserConfig& parser_config, const AppDetails& appDetails, const string& command_name)
{
    cout << appDetails.GetHeaderLine() << endl;
    if (!appDetails.Copyright.empty())
    {
        cout << appDetails.Copyright << endl;
    }

    auto parameters = arguments.GetArgumentsByType(ArgumentType::PARAMETER);
    parameters.sort(Argument::CompareByPosition);

    const auto optionsAndSwitchesTypes = { ArgumentType::PARAMETER, ArgumentType::OPTION, ArgumentType::SWITCH };
    auto optionsAndSwitches = arguments.GetArgumentsByTypes(optionsAndSwitchesTypes);
    optionsAndSwitches.sort(Argument::CompareByTypeAndPosition);

    const auto hasOptions = !optionsAndSwitches.empty();

    string argumentText;
    if (!command_name.empty())
    {
        argumentText
            .append(" ")
            .append(command_name);
    }

    for (auto iter = parameters.begin(); iter != parameters.end(); ++iter)
    {
        argumentText
            .append(" [")
            .append(iter->GetLongName())
            .append("]");
    }

    if (hasOptions)
    {
        argumentText.append(" [OPTIONS]");
    }

    cout << endl;
    cout << "Usage:" << endl;
    cout << AppDetails::GetApplicationName() << argumentText << endl;

    if (hasOptions)
    {
        cout << endl;
        cout << "Options:" << endl;

        list<tuple<string, string>> argument_details;

        size_t max_argument_details_length = 0;
        for (auto& argument : optionsAndSwitches)
        {
            auto details = GetArgumentDetails(argument, parser_config);
            auto description = GetArgumentDescription(argument);

            max_argument_details_length = max(details.length(), max_argument_details_length);

            const auto argument_detail = tuple(details, description);

            argument_details.push_back(argument_detail);
        }

        const auto padded_details_width = max_argument_details_length + 2;
        for (auto& tup : argument_details)
        {
            cout << left << setfill(' ') << setw(static_cast<streamsize>(padded_details_width)) << get<0>(tup)
                << get<1>(tup)
                << endl;
        }
    }

    list<string> argument_file_lines;
    if (parser_config.GetUseDefaultArgumentsFile())
    {
        const auto found = FileUtils::FileExists(AppDetails::GetDefaultCommandArgumentsFileName(command_name));

        argument_file_lines.push_back("Default App arguments can be specified in : " + AppDetails::GetDefaultCommandArgumentsFileName(command_name) + (found ? " (exists)" : ""));
    }
    if (parser_config.GetUseLocalArgumentsFile())
    {
        if (AppDetails::GetLocalCommandArgumentsFileName(command_name) != AppDetails::GetDefaultCommandArgumentsFileName(command_name))
        {
            const auto found = FileUtils::FileExists(AppDetails::GetLocalCommandArgumentsFileName(command_name));

            argument_file_lines.push_back("Local App arguments can be specified in : " + AppDetails::GetLocalCommandArgumentsFileName(command_name) + (found ? " (exists)" : ""));
        }
    }
    if (!argument_file_lines.empty())
    {
        cout << endl;
        for (const auto& line : argument_file_lines)
            cout << line << endl;
    }

    if (!arguments.GetNotes().empty())
    {
        cout << endl;
        cout << "Notes:" << endl;

        for (const auto& note: arguments.GetNotes())
        {
            cout << note << endl;
        }
    }
}

string ArgumentsUsageDisplay::GetArgumentDetails(const Argument& argument, const ParserConfig& parser_config)
{
    string details;

    if (argument.GetArgumentType() == ArgumentType::PARAMETER)
    {
        details = "[" + argument.GetLongName() + "]";
    }
    else
    {
        details = parser_config.GetShortNamePrefix() + argument.GetShortName();
        if (argument.HasLongName())
        {
            details += ", " + parser_config.GetLongNamePrefix() + argument.GetLongName();
        }
    }

    details += " " + ValueTypeTextConverter.GetText(argument.GetValueType());

    return details;
}

string ArgumentsUsageDisplay::GetArgumentDescription(const Argument& argument)
{
    auto description = argument.GetDescription();

    list<string> parts;

    if (argument.GetRequired())
    {
        parts.emplace_back("Required");
    }
    if (!argument.GetDefaultValue().empty())
    {
        parts.push_back("Default:" + argument.GetDefaultValue());
    }
    if (!argument.GetValueList().empty())
    {
        const auto valueListText = StringUtils::JoinText(argument.GetValueList(), ", ");

        parts.push_back("Values: " + valueListText);
    }

    if (!parts.empty())
    {
        description += " ("
            + StringUtils::JoinText(parts, ", ")
            + ")";
    }

    return description;
}

void ArgumentsUsageDisplay::ShowErrors(const Arguments& arguments, const int blankLinesBefore, const int blankLinesAfter)
{
    ConsoleUtils::ShowBlankLines(blankLinesBefore);

    const auto errors = arguments.GetErrors();
    for (auto& error : errors)
    {
        cout << ErrorLinePrefix << ": " << error << endl;
    }

    ConsoleUtils::ShowBlankLines(blankLinesAfter);
}
