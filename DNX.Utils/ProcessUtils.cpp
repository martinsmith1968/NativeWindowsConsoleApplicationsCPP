#include "stdafx.h"
#include "ProcessUtils.h"
#include "PathUtils.h"
#include "StringUtils.h"
#include <Windows.h>
#include <algorithm>

// ReSharper disable CppInconsistentNaming

using namespace std;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: ProcessUtils
//--------------------------------------------------------------------------

wstring ProcessUtils::GetExecutableFileNameWide()
{
    wchar_t buffer[MAX_PATH];
    const auto length = GetModuleFileName(nullptr, buffer, MAX_PATH);

    wstring wstr(buffer, length);

    return wstr;
}

string ProcessUtils::GetExecutableFileName()
{
    auto wstr = GetExecutableFileNameWide();

    return StringUtils::ToString(wstr);
}

string ProcessUtils::GetExecutableFileNameOnly()
{
    return StringUtils::AfterLast(GetExecutableFileName(), PathUtils::PATH_SEPARATOR);
}

string ProcessUtils::GetExecutableFilePath()
{
    return StringUtils::BeforeLast(GetExecutableFileName(), PathUtils::PATH_SEPARATOR);
}
