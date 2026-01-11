#include "stdafx.h"
#include "App.h"
#include <chrono>
#include <iostream>
#include <regex>

using namespace std;
using namespace std::chrono;
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
    const auto now = system_clock::now();

    const auto output = arguments.GetFormattedDateTime(now);

    cout << output << endl;
}
