#include "stdafx.h"
#include "ArgumentsParser.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/ListUtils.h"
#include "../DNX.Utils/StringUtils.h"
#include <fstream>
#include <list>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppClangTidyBugproneNarrowingConversions
// ReSharper disable CppClangTidyModernizePassByValue
// ReSharper disable CppTooWideScopeInitStatement

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;

//-----------------------------------------------------------------------------
// Internal methods
string ArgumentsParser::SanitizeText(const string& text)
{
    const auto double_quote = "\"";

    auto value = StringUtils::Trim(text);
    value = StringUtils::RemoveStartsAndEndsWith(value, double_quote);

    return value;
}

list<string> ArgumentsParser::ConvertLinesToRawArguments(const list<string>& lines)
{
    list<string> raw_arguments;

    const string quote = "\"";

    for (auto& line : lines)
    {
        string parts = StringUtils::Trim(line);

        if (StringUtils::StartsAndEndsWith(parts, quote) && StringUtils::CountOccurrences(parts, quote[0]) == 2)
        {
            raw_arguments.push_back(StringUtils::RemoveStartsAndEndsWith(parts, quote));
        }
        else
        {
            auto argument_name = SanitizeText(StringUtils::Before(parts, " "));
            auto argument_value = SanitizeText(StringUtils::After(parts, " "));

            raw_arguments.push_back(argument_name);
            raw_arguments.push_back(argument_value);
        }
    }

    return raw_arguments;
}

void ArgumentsParser::ParseArgumentsFile(Arguments& arguments, const string& fileName) const
{
    if (FileUtils::FileExists(fileName))
    {
        const auto arg = _parser_config.GetCustomArgumentsFilePrefix() + fileName;

        auto argumentValueConsumed = false;
        ParseArgument(arguments, arg, "", argumentValueConsumed);
    }
}

void ArgumentsParser::ParseArgumentsList(Arguments& arguments, list<string>& argumentsText) const
{
    for (auto i = 0; i < static_cast<int>(argumentsText.size()); ++i)
    {
        string argumentName  = ListUtils::GetAt(argumentsText,  i);
        string argumentValue = ListUtils::GetAt(argumentsText, i + 1);

        auto argumentValueConsumed = false;
        if (!ParseArgument(arguments, argumentName, argumentValue, argumentValueConsumed))
            return;
        if (argumentValueConsumed)
            i += 1;
    }
}

bool ArgumentsParser::ParseArgument(Arguments& arguments, const string& argumentName, const string& argumentValue, bool& argumentValueConsumed) const
{
    argumentValueConsumed = false;

    if (StringUtils::StartsWith(argumentName, _parser_config.GetCustomArgumentsFilePrefix()))
    {
        const auto fileName = StringUtils::RemoveStartsWith(argumentName, _parser_config.GetCustomArgumentsFilePrefix());

        try
        {
            const auto lines = FileUtils::ReadLines(fileName);
            auto argumentsText = ConvertLinesToRawArguments(lines);

            ParseArgumentsList(arguments, argumentsText);

            return true;
        }
        catch (exception& ex)
        {
            arguments.AddError(ex.what());
            return false;
        }
    }

    if (StringUtils::StartsWith(argumentName, _parser_config.GetLongNamePrefix()))
    {
        const auto switch_long_name = StringUtils::RemoveStartsWith(argumentName, _parser_config.GetLongNamePrefix(), 1);

        if (HandleAsSwitch(arguments, _parser_config, switch_long_name))
            return true;
        if (HandleAsOption(arguments, switch_long_name, argumentValue))
        {
            argumentValueConsumed = true;
            return true;
        }
    }

    if (StringUtils::StartsWith(argumentName, _parser_config.GetShortNamePrefix()))
    {
        const auto switch_short_name = StringUtils::RemoveStartsWith(argumentName, _parser_config.GetShortNamePrefix(), 1);

        if (HandleAsSwitch(arguments, _parser_config, switch_short_name))
            return true;
        if (HandleAsOption(arguments, switch_short_name, argumentValue))
        {
            argumentValueConsumed = true;
            return true;
        }
    }

    if (HandleAsParameter(arguments, arguments.GetNextPosition(), argumentName))
    {
        arguments.AdvancePosition();
        return true;
    }

    if (!_parser_config.GetIgnoreAdditionalArguments())
    {
        arguments.AddError("Unknown or unexpected argument: " + argumentName);
    }

    return false;
}

bool ArgumentsParser::HandleAsSwitch(Arguments& arguments, const ParserConfig& parser_config, const string& argumentName)
{
    const auto switchOnSuffix = string(1, parser_config.GetSwitchOnSuffix());
    const auto switchOffSuffix = string(1, parser_config.GetSwitchOffSuffix());

    auto switchValue = true;
    string switchName = argumentName;

    if (StringUtils::EndsWith(switchName, switchOnSuffix))
    {
        switchName = StringUtils::RemoveEndsWith(switchName, switchOnSuffix, 1);
        switchValue = true;
    }
    if (StringUtils::EndsWith(switchName, switchOffSuffix))
    {
        switchName = StringUtils::RemoveEndsWith(switchName, switchOffSuffix, 1);
        switchValue = false;
    }

    const auto& option = arguments.GetArgumentByName(argumentName);
    if (option.IsEmpty())
        return false;

    if (option.GetArgumentType() != ArgumentType::SWITCH)
    {
        return false;
    }

    arguments.SetArgumentValue(option.GetLongName(), StringUtils::BoolToString(switchValue));

    return true;
}

bool ArgumentsParser::HandleAsOption(Arguments& arguments, const string& argumentName, const string& argumentValue)
{
    const auto& option = arguments.GetArgumentByName(argumentName);
    if (option.IsEmpty())
        return false;

    if (option.GetArgumentType() != ArgumentType::OPTION)
    {
        return false;
    }

    arguments.SetArgumentValue(option.GetLongName(), argumentValue);

    return true;
}

bool ArgumentsParser::HandleAsParameter(Arguments& arguments, const int position, const string& argumentValue)
{
    auto& option = arguments.GetParameterAtPosition(position);
    if (option.IsEmpty())
        return false;

    arguments.SetArgumentValue(option.GetLongName(), argumentValue);

    return true;
}

void ArgumentsParser::ValidateRequired(Arguments& arguments)
{
    auto requiredArguments = arguments.GetRequiredArguments();

    for (auto iter = requiredArguments.begin(); iter != requiredArguments.end(); ++iter)
    {
        if (!arguments.HasArgumentValue(iter->GetShortName()))
        {
            arguments.AddError(iter->GetNameDescription() + " is required");
        }
    }
}

void ArgumentsParser::ValidateValues(Arguments& arguments)
{
    auto optionList = arguments.GetArguments();

    for (auto iter = optionList.begin(); iter != optionList.end(); ++iter)
    {
        const auto optionValue = arguments.GetArgumentValue(iter->GetShortName());
        if (!ValueConverter::IsValueValid(optionValue, iter->GetValueType()))
        {
            if (iter->GetValueType() == ValueType::STRING && optionValue.empty() && !iter->GetRequired())
                continue;

            arguments.AddError(iter->GetNameDescription() + " value is invalid (" + optionValue + ")");
        }
    }
}


//-----------------------------------------------------------------------------
// Public usage methods
ArgumentsParser::ArgumentsParser(Arguments& arguments, const AppDetails& app_details, const ParserConfig& parser_config, const ParserContext& parser_context)
    : _arguments(arguments),
    _app_details(app_details),
    _parser_config(parser_config),
    _parser_context(parser_context)

{
}

void ArgumentsParser::Parse(const int argc, char* argv[]) const
{
    Parse(ListUtils::ToList(argc, argv, 1));
}

void ArgumentsParser::Parse(list<string> arguments) const
{
    if (_parser_config.GetUseCustomArgumentsFile() && _arguments.IsUsingDefaultArgumentsFile())
        ParseArgumentsFile(_arguments, _parser_context.GetDefaultArgumentsFileName());

    if (_parser_config.GetUseLocalArgumentsFile() && _arguments.IsUsingDefaultArgumentsFile())
        ParseArgumentsFile(_arguments, _parser_context.GetLocalArgumentsFileName());

    ParseArgumentsList(_arguments, arguments);

    // Validate
    ValidateRequired(_arguments);
    ValidateValues(_arguments);
    _arguments.PostParseValidate();
}

//-----------------------------------------------------------------------------
// Static Public methods
void ArgumentsParser::ParseArguments(Arguments& arguments, const int argc, char* argv[], const AppDetails& app_details, const ParserConfig& parser_config, const ParserContext& parser_context)
{
    const auto parser = ArgumentsParser(arguments, app_details, parser_config, parser_context);
    parser.Parse(argc, argv);
}

void ArgumentsParser::ParseArguments(Arguments& arguments, const list<string>& args, const AppDetails& app_details, const ParserConfig& parser_config, const ParserContext& parser_context)
{
    const auto parser = ArgumentsParser(arguments, app_details, parser_config, parser_context);
    parser.Parse(args);
}
