#include "stdafx.h"
#include "App.h"
#include "../DNX.Utils/DateUtils.h"
#include <iostream>
#include <conio.h>
#include <chrono>
#include <regex>
#include <thread>

using namespace std;
using namespace PauseN;

// ReSharper disable CppTooWideScope
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppTooWideScopeInitStatement

//------------------------------------------------------------------------------
// Execute
void App::Execute(AppArguments& arguments)
{
    cout << arguments.GetFormattedMessageText();

    const auto start_time = DateUtils::GetNow();
    const auto exit_time = start_time + arguments.GetTimeoutSeconds();
    const auto sleep_time = std::chrono::milliseconds(arguments.GetSleepMilliseconds());

    do
    {
        if (_kbhit())
        {
            _getch();
            break;
        }

        std::this_thread::sleep_for(sleep_time);
    } while (DateUtils::GetNow() < exit_time || arguments.GetTimeoutSeconds() == 0);

    cout << endl;
}
