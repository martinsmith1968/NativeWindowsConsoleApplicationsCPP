#include "stdafx.h"
#include "DateUtils.h"
#include "EnvironmentUtils.h"
#include "FileUtils.h"
#include "PathUtils.h"
#include "ProcessUtils.h"
#include "StringUtils.h"
#include <direct.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <sys/stat.h>


// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl

using namespace std;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: PathUtils
//--------------------------------------------------------------------------

const string PathUtils::DRIVE_SEPARATOR = ":";
const string PathUtils::PATH_SEPARATOR  = "\\";

const string PathUtils::RELATIVE_DIRECTORY_CURRENT = ".";
const string PathUtils::RELATIVE_DIRECTORY_PARENT  = "..";
const string PathUtils::RELATIVE_DIRECTORY_ROOT    = "\\";

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

string PathUtils::Combine(const string& path1, const string& path2, const string& path3)
{
    return Combine(Combine(path1, path2), path3);
}

string PathUtils::Combine(const string& path1, const string& path2, const string& path3, const string& path4)
{
    return Combine(Combine(path1, path2, path3), path4);
}

string PathUtils::Combine(const string& path1, const string& path2, const string& path3, const string& path4, const string& path5)
{
    return Combine(Combine(path1, path2, path3, path4), path5);
}

string PathUtils::Combine(const string& path1, const string& path2, const string& path3, const string& path4, const string& path5, const string& path6)
{
    return Combine(Combine(path1, path2, path3, path4, path5), path6);
}

string PathUtils::Combine(const string& path1, const string& path2, const string& path3, const string& path4, const string& path5, const string& path6, const string& path7)
{
    return Combine(Combine(path1, path2, path3, path4, path5, path6), path7);
}

string PathUtils::Combine(const string& path1, const string& path2, const string& path3, const string& path4, const string& path5, const string& path6, const string& path7, const string& path8)
{
    return Combine(Combine(path1, path2, path3, path4, path5, path6, path7), path8);
}

string PathUtils::GetAbsolutePath(const string& path)
{
    // TODO: implement this properly
    return path;
}

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

bool PathUtils::DirectoryExists(const string& path)
{
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
        return false;

    return (info.st_mode & S_IFDIR);
}

bool PathUtils::CreateDirectory(const string& path)
{
    if (StringUtils::Trim(path).empty())
        return false;

    if (DirectoryExists(path))
        return true;

    const auto parent = StringUtils::BeforeLast(StringUtils::RemoveEndsWith(path, PATH_SEPARATOR), PATH_SEPARATOR);
    if (parent.empty() || StringUtils::EndsWith(parent, DRIVE_SEPARATOR))
        return true;

    if (!CreateDirectory(parent))
        return false;

    _mkdir(path.c_str());

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

    //if (!CreateDirectory(part))
    //    return false;
    //
    //if (DirectoryExists(part))
    //    return true;
    //
    //_mkdir(part.c_str());

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
