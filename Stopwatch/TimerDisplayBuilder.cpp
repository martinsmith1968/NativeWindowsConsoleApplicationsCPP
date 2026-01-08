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
const string TimerDisplayBuilder::DefaultStartTimeTextFormat = "%Y-%m-%d %H:%M:%S";

string TimerDisplayBuilder::GetFormattedDisplayText(const Timer& timer, const string& format, const string& action)
{
    auto text = format;

    text = StringUtils::Replace(text, "{name}", timer.GetName());
    text = StringUtils::Replace(text, "{action}", action);
    text = StringUtils::Replace(text, "{state}", TimerStateTypeTextResolver().GetText(timer.GetState()));

    text = GetFormattedStartTime(timer.GetStartDateTime(), text);

    text = GetFormattedElapsedTime(timer.GetAccumulatedElapsed(), text);

    return text;
}

string TimerDisplayBuilder::GetFormattedElapsedTime(const double timespan, const string& format)
{
    auto text = format;

    const auto elapsed_time = ElapsedTime(timespan);

    text = StringUtils::Replace(text, "{days}", to_string(elapsed_time.GetDays()));
    text = StringUtils::Replace(text, "{hours}", to_string(elapsed_time.GetHours()));
    text = StringUtils::Replace(text, "{minutes}", to_string(elapsed_time.GetMinutes()));
    text = StringUtils::Replace(text, "{seconds}", to_string(elapsed_time.GetSeconds()));

    text = StringUtils::Replace(text, "0 days, 0 hours, 0 minutes, ", "");
    text = StringUtils::Replace(text, "0 days, 0 hours, ", "");
    text = StringUtils::Replace(text, "0 days, ", "");

    return text;
}

string TimerDisplayBuilder::GetFormattedStartTime(const tm& tm, const string& format)
{
    return DateUtils::FormatDate(&tm, format);
}
