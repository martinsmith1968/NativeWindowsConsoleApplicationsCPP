#pragma once

#include "stdafx.h"
#include "Timer.h"
#include <string>
#include <iomanip>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace Stopwatch
{
    class TimerDisplayBuilder
    {
    public:
        static const string DefaultElapsedTimeTextFormat;
        static const string DefaultStartTimeTextFormat;

        [[nodiscard]] static string GetFormattedDisplayText(const Timer& timer, const string& format, const string& action = "");
        [[nodiscard]] static string GetFormattedElapsedTime(double timespan, const string& format);
        [[nodiscard]] static string GetFormattedStartTime(const tm& tm, const string& format);
    };
}
