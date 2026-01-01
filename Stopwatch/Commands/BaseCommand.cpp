#include "stdafx.h"
#include "BaseCommand.h"
#include "CancelCommand.h"
#include "ElapsedCommand.h"
#include "ListCommand.h"
#include "PauseCommand.h"
#include "PurgeCommand.h"
#include "ResumeCommand.h"
#include "StartCommand.h"
#include "StopCommand.h"
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
    AddSwitch("v", ArgumentNameVerboseOutput, default_value, "Control verbosity of output messages", false, APP_MAX - 10);
}
void BaseArguments::AddSwitchQuiet(const bool default_value)
{
    AddSwitch("q", ArgumentNameQuiet, default_value, "Suppress output messages", false, APP_MAX - 15);
}
void BaseArguments::AddSwitchIgnoreInvalidState(const bool default_value)
{
    AddSwitch("i", ArgumentNameIgnoreInvalidState, default_value, "Ignore errors of Stopwatch being in invalid state for the action", false, APP_MAX - 20);
}
void BaseArguments::AddSwitchShowElapsedTime(const bool default_value)
{
    AddSwitch("set", ArgumentNameShowElapsedTime, default_value, "Show the Stopwatch Elapsed Time", false, APP_MAX - 35);
}
void BaseArguments::AddOptionElapsedTimeDisplayFormat()
{
    AddOption(ValueType::STRING, "etdf", ArgumentNameElapsedTimeDisplayFormat, "{name}: {state} - " + TimerDisplayBuilder::DefaultElapsedTimeTextFormat, "The format string to use to display Elapsed Time", false, APP_MAX - 30);
}
void BaseArguments::AddOptionElapsedTimeAlternativeDisplayFormat()
{
    AddOption(ValueType::STRING, "etadf", ArgumentNameElapsedTimeDisplayFormat, "{name}: {action} - " + TimerDisplayBuilder::DefaultElapsedTimeTextFormat, "The format string to use to display Elapsed Time", false, APP_MAX - 35);
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

const ParserContext CancelArguments::m_parser_context  = ParserContext(StringUtils::ToLower(CommandTypeTextResolver().GetText(CommandType::CANCEL)));
const ParserContext ElapsedArguments::m_parser_context = ParserContext(StringUtils::ToLower(CommandTypeTextResolver().GetText(CommandType::ELAPSED)));
const ParserContext ListArguments::m_parser_context    = ParserContext(StringUtils::ToLower(CommandTypeTextResolver().GetText(CommandType::LIST)));
const ParserContext PauseArguments::m_parser_context   = ParserContext(StringUtils::ToLower(CommandTypeTextResolver().GetText(CommandType::PAUSE)));
const ParserContext PurgeArguments::m_parser_context   = ParserContext(StringUtils::ToLower(CommandTypeTextResolver().GetText(CommandType::PAUSE)));
const ParserContext ResumeArguments::m_parser_context  = ParserContext(StringUtils::ToLower(CommandTypeTextResolver().GetText(CommandType::RESUME)));
const ParserContext StartArguments::m_parser_context   = ParserContext(StringUtils::ToLower(CommandTypeTextResolver().GetText(CommandType::START)));
const ParserContext StopArguments::m_parser_context    = ParserContext(StringUtils::ToLower(CommandTypeTextResolver().GetText(CommandType::STOP)));
