#include "stdafx.h"
#include "TimerDisplayBuilder.h"
#include "ElapsedTime.h"
#include "Timer.h"
#include "../DNX.Utils/DateUtils.h"
#include "../DNX.Utils/StringUtils.h"

// ReSharper disable CppInconsistentNaming

using namespace std;
using namespace DNX::Utils;
using namespace Stopwatch;

const string TimerDisplayBuilder::DefaultElapsedTimeTextFormat = "{days} days, {hours} hours, {minutes} minutes, {seconds} seconds";
const string TimerDisplayBuilder::DefaultStartTimeTextFormat   = "%Y-%m-%d %H:%M:%S";

string TimerDisplayBuilder::GetFormattedText(const Timer& timer, const string& format)
{
    auto text = format;

    text = StringUtils::ReplaceString(text, "{name}", timer.GetName());
    text = StringUtils::ReplaceString(text, "{state}", TimerStateTypeTextResolver().GetText(timer.GetState()));

    text = GetFormattedStartTime(timer.GetStartDateTime(), text);

    text = GetFormattedElapsedTime(timer.GetAccumulatedElapsed(), text);

    return text;
}

string TimerDisplayBuilder::GetFormattedElapsedTime(const double timespan, const string& format)
{
    auto text = format;

    const auto elapsed_time = ElapsedTime(timespan);

    text = StringUtils::ReplaceString(text, "{days}", to_string(elapsed_time.GetDays()));
    text = StringUtils::ReplaceString(text, "{hours}", to_string(elapsed_time.GetHours()));
    text = StringUtils::ReplaceString(text, "{minutes}", to_string(elapsed_time.GetMinutes()));
    text = StringUtils::ReplaceString(text, "{seconds}", to_string(elapsed_time.GetSeconds()));

    text = StringUtils::ReplaceString(text, "0 days, 0 hours, 0 minutes, ", "");
    text = StringUtils::ReplaceString(text, "0 days, 0 hours, ", "");
    text = StringUtils::ReplaceString(text, "0 days, ", "");

    return text;
}

string TimerDisplayBuilder::GetFormattedStartTime(const tm& tm, const string& format)
{
    return DateUtils::FormatDate(&tm, format);
}
