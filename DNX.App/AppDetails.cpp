#include "stdafx.h"
#include "AppDetails.h"
#include "../DNX.Utils/DirectoryUtils.h"
#include "../DNX.Utils/ProcessUtils.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/PathUtils.h"
#include "../DNX.Utils/StringUtils.h"
#include <sstream>

// ReSharper disable CppInconsistentNaming

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;

AppDetails::AppDetails() = default;

AppDetails::~AppDetails() = default;

string AppDetails::GetVersionDetails() const
{
    return Version.empty()
        ? ""
        : "v" + Version;
}

string AppDetails::_arguments_file_extension = "options";

string AppDetails::GetHeaderLine() const
{
    ostringstream text;

    text << Name;

    if (!Version.empty())
    {
        text << " " << GetVersionDetails();
    }

    if (!Description.empty())
    {
        text << " - " << Description;
    }

    return text.str();
}

string AppDetails::GetApplicationName()
{
    return PathUtils::GetFileNameOnly(ProcessUtils::GetExecutableFileNameOnly());
}

string AppDetails::GetDefaultArgumentsFileExtension()
{
    return _arguments_file_extension;
}

string AppDetails::GetArgumentsFileNameOnly()
{
    const auto executableName = ProcessUtils::GetExecutableFileNameOnly();

    return PathUtils::ChangeFileExtension(executableName, _arguments_file_extension);
}

string AppDetails::GetArgumentsFileNameOnly(const string& file_name_suffix)
{
    const auto executableName = ProcessUtils::GetExecutableFileNameOnly();

    return PathUtils::ChangeFileExtension(executableName, file_name_suffix + "." + _arguments_file_extension);

}

string AppDetails::GetDefaultArgumentsFileName()
{
    return PathUtils::Combine(ProcessUtils::GetExecutableFilePath(), GetArgumentsFileNameOnly());
}

string AppDetails::GetLocalArgumentsFileName()
{
    return PathUtils::Combine(DirectoryUtils::GetCurrentDirectory(), GetArgumentsFileNameOnly());
}

string AppDetails::GetDefaultCommandArgumentsFileName(const string& command_name)
{
    const auto fileName = GetArgumentsFileNameOnly(StringUtils::ToLower(command_name));
    return PathUtils::Combine(ProcessUtils::GetExecutableFilePath(), fileName);
}

string AppDetails::GetLocalCommandArgumentsFileName(const string& command_name)
{
    const auto fileName = GetArgumentsFileNameOnly(StringUtils::ToLower(command_name));
    return PathUtils::Combine(DirectoryUtils::GetCurrentDirectory(), fileName);
}
