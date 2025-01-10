#include "stdafx.h"
#include "App.h"
#include <iostream>

using namespace std;
using namespace BannerText;

// ReSharper disable CppTooWideScope
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppTooWideScopeInitStatement

void App::Execute(AppArguments& arguments)
{
    const auto header_line_count = arguments.GetHeaderLineCount();
    if (header_line_count > 0)
    {
        const auto header_line = arguments.GetHeaderLine();

        for (auto i = 0; i < static_cast<int>(header_line_count); ++i)
        {
            cout << header_line << endl;
        }
    }

    const auto text_lines = arguments.GetTextLines();
    for (auto& line: text_lines)
    {
        cout << line << endl;
    }

    const auto footer_line_count = arguments.GetFooterLineCount();
    if (footer_line_count)
    {
        const auto footer_line = arguments.GetFooterLine();

        for (auto i = 0; i < static_cast<int>(footer_line_count); ++i)
        {
            cout << footer_line << endl;
        }
    }
}
