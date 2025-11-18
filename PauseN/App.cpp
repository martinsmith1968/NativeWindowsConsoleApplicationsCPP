#include "stdafx.h"
#include "App.h"
#include "../DNX.Utils/DateUtils.h"
#include <iostream>
#include <conio.h>
#include <chrono>
#include <regex>
#include <thread>
#include <Windows.h>

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

    auto wait_forever = false;
    do
    {
        if (_kbhit())
        {
            const auto key = _getch();
            if (arguments.GetEscapeCancelsTimeout() && key == VK_ESCAPE && !wait_forever)
            {
                wait_forever = true;
                cout << arguments.GetTimeoutCancelledText();
            }
            else
            {
                break;
            }
        }

        std::this_thread::sleep_for(sleep_time);
    } while (wait_forever || DateUtils::GetNow() < exit_time || arguments.GetTimeoutSeconds() == 0);

    cout << endl;
}
