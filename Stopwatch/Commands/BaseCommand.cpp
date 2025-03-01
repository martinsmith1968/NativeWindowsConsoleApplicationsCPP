#include "stdafx.h"
#include "BaseCommand.h"
#include "../../DNX.Utils/StringUtils.h"

// ReSharper disable CppInconsistentNaming

using namespace std;
using namespace Stopwatch;


BaseArguments::BaseArguments(const ParserContext& parser_context)
    : Arguments(parser_context)
{
    AddOption(ValueType::STRING, "df", ArgumentNameDataFileName, TimerRepository::GetDefaultRepositoryFileName(), "The filename to store Stopwatch data in", false, APP_MAX);
}

void BaseArguments::AddParameterStopwatchName()
{
    AddParameter(ValueType::STRING, 1, ArgumentNameStopwatchName, "", "The name of the Stopwatch", true);
}
void BaseArguments::AddOptionAdditionalText()
{
    AddOption(ValueType::STRING, "at", ArgumentNameAdditionalText, "", "Additional text for the output", false, APP_MAX - 5);
}
void BaseArguments::AddSwitchVerboseOutput(const bool default_value)
{
    AddSwitch("v", ArgumentNameVerboseOutput, StringUtils::BoolToString(default_value), "Control verbosity of output messages", false, APP_MAX - 10);
}
void BaseArguments::AddSwitchIgnoreInvalidState(const bool default_value)
{
    AddSwitch("i", ArgumentNameIgnoreInvalidState, StringUtils::BoolToString(default_value), "Ignore errors of Stopwatch being in invalid state for the action", false, APP_MAX - 20);
}
void BaseArguments::AddSwitchShowElapsedTime(const bool default_value)
{
    AddSwitch("set", ArgumentNameShowElapsedTime, StringUtils::BoolToString(default_value), "Show the Stopwatch Elapsed Time", false, APP_MAX - 35);
}
void BaseArguments::AddOptionElapsedTimeDisplayFormat()
{
    AddOption(ValueType::STRING, "etdf", ArgumentNameElapsedTimeDisplayFormat, "{name}: {state} - " + TimerDisplayBuilder::DefaultElapsedTimeTextFormat, "The format string to use to display Elapsed Time", false, APP_MAX - 34);
}
void BaseArguments::AddOptionElapsedTimeAlternativeDisplayFormat()
{
    AddOption(ValueType::STRING, "etdf", ArgumentNameElapsedTimeDisplayFormat, "{name}: {action} - " + TimerDisplayBuilder::DefaultElapsedTimeTextFormat, "The format string to use to display Elapsed Time", false, APP_MAX - 34);
}

string BaseArguments::GetDataFileName()
{
    return GetArgumentValue(ArgumentNameDataFileName);
}

string BaseArguments::GetStopwatchName()
{
    return GetArgumentValue(ArgumentNameStopwatchName);
}
bool BaseArguments::GetVerboseOutput()
{
    return GetSwitchValue(ArgumentNameVerboseOutput);
}
bool BaseArguments::GetIgnoreInvalidState()
{
    return GetSwitchValue(ArgumentNameIgnoreInvalidState);
}
bool BaseArguments::GetShowElapsedTime()
{
    return GetSwitchValue(ArgumentNameShowElapsedTime);
}
string BaseArguments::GetElapsedTimeDisplayFormat()
{
    return GetArgumentValue(ArgumentNameElapsedTimeDisplayFormat);
}
string BaseArguments::GetArgumentAdditionalText()
{
    return GetArgumentValue(ArgumentNameAdditionalText);
}
