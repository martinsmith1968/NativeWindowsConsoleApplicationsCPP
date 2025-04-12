#include "stdafx.h"
#include "DateUtils.h"
#include "PathUtils.h"
#include "ProcessUtils.h"
#include "StringUtils.h"
#include <direct.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <sys/stat.h>

#include "EnvironmentUtils.h"
#include "FileUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl

using namespace std;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: PathUtils
//--------------------------------------------------------------------------

const string PathUtils::DRIVE_SEPARATOR = ":";
const string PathUtils::PATH_SEPARATOR = "\\";

string PathUtils::GetCurrentDirectory()
{
    char cwd[_MAX_PATH];
    _getcwd(cwd, _MAX_PATH);

    return cwd;
}

string PathUtils::ChangeDirectory(const string& path)
{
    _chdir(path.c_str());

    return GetCurrentDirectory();
}

string PathUtils::Combine(const string& path1, const string& path2)
{
    auto path = path1.empty()
        ? path1
        : StringUtils::RemoveEndsWith(path1, PATH_SEPARATOR);

    if (!path.empty() && !path2.empty())
    {
        path += PATH_SEPARATOR;
    }

    path += path2;

    return path;
}

bool PathUtils::DirectoryExists(const string& path)
{
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
        return false;

    return (info.st_mode & S_IFDIR);
}

bool PathUtils::CreateDirectory(const string& path)
{
    const auto part = StringUtils::BeforeLast(path, PATH_SEPARATOR);
    if (part.empty() || StringUtils::EndsWith(part, DRIVE_SEPARATOR))
        return true;

    if (!CreateDirectory(part))
        return false;

    if (DirectoryExists(part))
        return true;

    _mkdir(part.c_str());
    return true;
}

bool PathUtils::DeleteDirectory(const string& path, bool recurse_sub_directories, bool remove_files)
{
    // TODO: recursion needs implementing

    if (!DirectoryExists(path))
        return false;

    for (const auto& entry : std::filesystem::directory_iterator(path))
        std::cout << entry.path() << std::endl;


    const auto part = StringUtils::BeforeLast(path, PATH_SEPARATOR);
    if (part.empty() || StringUtils::EndsWith(part, DRIVE_SEPARATOR))
        return true;

    if (!CreateDirectory(part))
        return false;

    if (DirectoryExists(part))
        return true;

    _mkdir(part.c_str());
    return true;
}
string PathUtils::GetTempPath()
{
    string path = EnvironmentUtils::GetEnvironmentVariableValue("TEMP");
    if (StringUtils::Trim(path).empty())
    {
        path = EnvironmentUtils::GetEnvironmentVariableValue("TMP");
    }
    if (StringUtils::Trim(path).empty())
    {
        path = Combine(GetUserHomeDirectory(), "Temp");
    }

    if (!path.empty())
    {
        if (!DirectoryExists(path))
        {
            CreateDirectory(path);
        }
    }

    return path;
}

string PathUtils::GetTempFileName(const string& prefix, const string& extension)
{
    const auto now = DateUtils::ToCalendarDateTime(DateUtils::GetNow());

    const auto filePath = GetTempPath();

    const auto filePrefix = StringUtils::Trim(prefix).empty()
        ? ProcessUtils::GetExecutableFileNameOnly()
        : prefix;

    string fileName;
    auto adjuster = 0;
    do
    {
        auto fileId = to_string(now.tm_hour) + to_string(now.tm_min) + to_string(now.tm_sec)
            + (adjuster++ > 0
                ? to_string(adjuster)
                : "");

        fileName = Combine(filePath, filePrefix + fileId + "." + extension);
    } while (FileUtils::FileExists(fileName));

    FileUtils::Create(fileName);

    return fileName;
}

string PathUtils::GetUserHomeDirectory()
{
    string path = EnvironmentUtils::GetEnvironmentVariableValue("HOME");
    if (StringUtils::Trim(path).empty())
    {
        path = EnvironmentUtils::GetEnvironmentVariableValue("USERPROFILE");
    }
    if (StringUtils::Trim(path).empty())
    {
        path = Combine(EnvironmentUtils::GetEnvironmentVariableValue("HOMEDRIVE"), EnvironmentUtils::GetEnvironmentVariableValue("HOMEPATH"));
    }

    return path;
}

string PathUtils::GetUserDataDirectory()
{
    auto path = GetUserHomeDirectory();
    path = Combine(path, "AppData");
    path = Combine(path, "Local");
    return path;
}
