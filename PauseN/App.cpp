#include "stdafx.h"
#include "App.h"
#include <iostream>
#include <conio.h>
#include <chrono>
#include <ctime>
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

    const auto start_time = time(nullptr);
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
    } while (time(nullptr) < exit_time || arguments.GetTimeoutSeconds() == 0);

    cout << endl;
}
