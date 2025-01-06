#include "stdafx.h"
#include "DateUtils.h"
#include <ctime>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCertMsc51Cpp
// ReSharper disable CppClangTidyClangDiagnosticShorten64To32

using namespace std;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: Date
//--------------------------------------------------------------------------

string DateUtils::FormatDate(::tm const* tm, const string& format)
{
    constexpr int BUFFER_SIZE = 1024;

    char buffer[BUFFER_SIZE];

    const auto size = strftime(buffer, BUFFER_SIZE, format.c_str(), tm);

    return buffer;
}
