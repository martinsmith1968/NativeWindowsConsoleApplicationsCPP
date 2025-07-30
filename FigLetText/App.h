#pragma once

#include "AppArguments.h"

// ReSharper disable CppInconsistentNaming

namespace FigLetText
{
    class App
    {
        static list<string> GetLines(const string& text);

    public:
        static void Execute(AppArguments& arguments);
    };
}
