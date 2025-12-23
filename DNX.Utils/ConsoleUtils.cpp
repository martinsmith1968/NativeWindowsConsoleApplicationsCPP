#include "stdafx.h"
#include "ConsoleUtils.h"
#include <Windows.h>
#include <iostream>

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming

using namespace DNX::Utils;

void ConsoleUtils::ShowBlankLines(const int count)
{
    for (auto iter = 0; iter < count; ++iter)
    {
        cout << endl;
    }
}

// Source : https://stackoverflow.com/questions/23369503/get-size-of-terminal-window-rows-columns
int ConsoleUtils::GetConsoleWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    const auto width = (int)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
    const auto height = (int)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);

    return width;
}
