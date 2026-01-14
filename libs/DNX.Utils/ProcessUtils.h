#pragma once

#include "stdafx.h"
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::Utils
{
    //--------------------------------------------------------------------------
    // Class: ProcessUtils
    //--------------------------------------------------------------------------
    class ProcessUtils
    {
    public:
        static wstring GetExecutableFileNameWide();

        static string GetExecutableFileName();
        static string GetExecutableFileNameOnly();
        static string GetExecutableFilePath();
    };
}
