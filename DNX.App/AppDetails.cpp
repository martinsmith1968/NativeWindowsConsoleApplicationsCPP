#include "stdafx.h"
#include "AppDetails.h"
#include "../DNX.Utils/ProcessUtils.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/PathUtils.h"
#include <sstream>

// ReSharper disable CppInconsistentNaming

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;

AppDetails::AppDetails() = default;

AppDetails::~AppDetails() = default;

string AppDetails::_arguments_file_extension = "options";

string AppDetails::GetHeaderLine() const
{
    ostringstream text;

    text << Name;

    if (!Version.empty())
    {
        text << " v" << Version;
    }

    if (!Description.empty())
    {
        text << " - " << Description;
    }

    return text.str();
}

string AppDetails::GetApplicationName()
{
    return FileUtils::GetFileNameOnly(ProcessUtils::GetExecutableFileNameOnly());
}

string AppDetails::GetDefaultArgumentsFileExtension()
{
    return _arguments_file_extension;
}

string AppDetails::GetArgumentsFileNameOnly()
{
    const auto executableName = ProcessUtils::GetExecutableFileNameOnly();

    return FileUtils::ChangeFileExtension(executableName, _arguments_file_extension);
}

string AppDetails::GetDefaultArgumentsFileName()
{
    return PathUtils::Combine(ProcessUtils::GetExecutableFilePath(), GetArgumentsFileNameOnly());
}

string AppDetails::GetLocalArgumentsFileName()
{
    return PathUtils::Combine(PathUtils::GetCurrentDirectory(), GetArgumentsFileNameOnly());
}

string AppDetails::GetDefaultCommandArgumentsFileName(const Command& command)
{
    const auto fileName = GetArgumentsFileNameOnly() + "." + command.GetName();
    return PathUtils::Combine(ProcessUtils::GetExecutableFilePath(), fileName);
}

string AppDetails::GetLocalCommandArgumentsFileName(const Command& command)
{
    const auto fileName = GetArgumentsFileNameOnly() + "." + command.GetName();
    return PathUtils::Combine(PathUtils::GetCurrentDirectory(), fileName);
}
