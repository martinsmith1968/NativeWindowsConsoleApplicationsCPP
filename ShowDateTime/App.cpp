#include "stdafx.h"
#include "App.h"
#include "../DNX.Utils/DateUtils.h"
#include <iostream>
#include <regex>

using namespace std;
using namespace ShowDateTime;

// ReSharper disable CppTooWideScope
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppClangTidyConcurrencyMtUnsafe

//------------------------------------------------------------------------------
// Execute
void App::Execute(AppArguments& arguments)
{
    const auto now = DateUtils::GetNow();

    tm datetime;
     auto err = localtime_s(&datetime, &now);

    if (arguments.GetTimeType() == TimeType::UTC)
    {
        err = gmtime_s(&datetime, &now);
    }

    const auto output = arguments.GetFormattedDateTime(&datetime);

    cout << output << endl;
}
