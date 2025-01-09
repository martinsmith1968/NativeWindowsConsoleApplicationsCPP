#include "stdafx.h"
#include "ArgumentsUsageDisplay.h"
#include "../DNX.Utils/ConsoleUtils.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/StringUtils.h"
#include <iomanip>
#include <iostream>
#include <ostream>

#include "ParserConfig.h"

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
        cout << "OPTIONS:" << endl;

        list<tuple<Argument, string, string>> argumentDescriptions;

        size_t maxArgumentDescriptionLength = 0;
        for (auto iter = optionsAndSwitches.begin(); iter != optionsAndSwitches.end(); ++iter)
        {
            string argument_description;
            if (iter->GetArgumentType() == ArgumentType::PARAMETER)
            {
                argument_description = "[" + iter->GetLongName() + "]";
            }
            else
            {
                argument_description = "-" + iter->GetShortName();
                if (iter->HasLongName())
                {
                    argument_description += ", --" + iter->GetLongName();
                }
            }
            argument_description += " " + ValueTypeTextConverter.GetText(iter->GetValueType());

            maxArgumentDescriptionLength = max(argument_description.length(), maxArgumentDescriptionLength);

            auto textDesc = iter->GetDescription();

            list<string> textDescParts;

            if (iter->GetRequired())
            {
                textDescParts.emplace_back("Required");
            }
            if (!iter->GetDefaultValue().empty())
            {
                textDescParts.push_back("Default:" + iter->GetDefaultValue());
            }
            if (!iter->GetValueList().empty())
            {
                const auto valueListText = StringUtils::JoinText(iter->GetValueList(), ", ");

                textDescParts.push_back("Values: " + valueListText);
            }

            if (!textDescParts.empty())
            {
                textDesc += " (";

                auto index = 0;
                for (auto textPart = textDescParts.begin(); textPart != textDescParts.end(); ++textPart)
                {
                    if (index > 0)
                    {
                        textDesc += ", ";
                    }

                    textDesc += *textPart;

                    ++index;
                }

                textDesc += ")";
            }

            const auto optionAndDesc = tuple(*iter, argument_description, textDesc);

            argumentDescriptions.push_back(optionAndDesc);
        }

        const auto paddedWidth = maxArgumentDescriptionLength + 2;
        for (auto iter = argumentDescriptions.begin(); iter != argumentDescriptions.end(); ++iter)
        {
            cout << left << setfill(' ') << setw(static_cast<streamsize>(paddedWidth)) << get<1>(*iter)
                << get<2>(*iter)
                << endl;
        }
    }

    list<string> argument_file_lines;
    if (parser_config.GetUseDefaultArgumentsFile())
    {
        const auto found = FileUtils::FileExists(AppDetails::GetDefaultArgumentsFileName());

        argument_file_lines.push_back("Default App arguments can be specified in : " + AppDetails::GetDefaultArgumentsFileName() + (found ? " (exists)" : ""));
    }
    if (parser_config.GetUseLocalArgumentsFile())
    {
        if (AppDetails::GetLocalArgumentsFileName() != AppDetails::GetDefaultArgumentsFileName())
        {
            const auto found = FileUtils::FileExists(AppDetails::GetLocalArgumentsFileName());

            argument_file_lines.push_back("Local App arguments can be specified in : " + AppDetails::GetLocalArgumentsFileName() + (found ? " (exists)" : ""));
        }
    }

    if (!argument_file_lines.empty())
    {
        cout << endl;
        for (const auto& line : argument_file_lines)
            cout << line << endl;
    }
}

void ArgumentsUsageDisplay::ShowErrors(const Arguments& arguments, const int blankLinesBefore, const int blankLinesAfter)
{
    ConsoleUtils::ShowBlankLines(blankLinesBefore);

    auto errors = arguments.GetErrors();
    for (auto iter = errors.begin(); iter != errors.end(); ++iter)
    {
        cout << ErrorLinePrefix << ": " << *iter << endl;
    }

    ConsoleUtils::ShowBlankLines(blankLinesAfter);
}
