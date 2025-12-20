#include "stdafx.h"
#include "DateTime.h"
#include "DirectoryUtils.h"
#include "EnvironmentUtils.h"
#include "PathUtils.h"
#include "StringUtils.h"
#include <cstdlib>
#include <direct.h>
#include <exception>
#include <iostream>
#include <filesystem>
#include <list>
#include <string>
#include <system_error>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl

using namespace std;
using namespace std::filesystem;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: DirectoryUtils
//--------------------------------------------------------------------------

string DirectoryUtils::GetCurrentDirectory()
{
    char cwd[_MAX_PATH];
    auto _ = _getcwd(cwd, _MAX_PATH);

    return cwd;
}

string DirectoryUtils::GetCurrentDirectory(const string& drive_reference)
{
    if (!PathUtils::IsDriveReference(drive_reference))
        return GetCurrentDirectory();

    const auto current_drive = GetCurrentDrive();
    SetCurrentDrive(drive_reference);

    const auto current_directory = GetCurrentDirectory();
    SetCurrentDrive(current_drive);

    return current_directory;
}

string DirectoryUtils::SetCurrentDirectory(const string& path)
{
    auto _ = _chdir(path.c_str());

    return GetCurrentDirectory();
}

string DirectoryUtils::GetCurrentDrive()
{
    return PathUtils::GetDriveReference(GetCurrentDriveId());
}

int DirectoryUtils::GetCurrentDriveId()
{
    const auto drive_id = _getdrive();

    return drive_id;
}

bool DirectoryUtils::SetCurrentDrive(const string& drive_reference)
{
    const auto drive_id = PathUtils::GetDriveId(drive_reference);

    return SetCurrentDrive(drive_id);
}

bool DirectoryUtils::SetCurrentDrive(const int drive_id)
{
    if (drive_id < 1 || drive_id > 26)
        return false;

    if (_chdrive(drive_id) != 0)
        return false;

    return true;
}

string DirectoryUtils::GetUserHomeDirectory()
{
    string path = EnvironmentUtils::GetEnvironmentVariableValue("HOME");
    if (StringUtils::Trim(path).empty())
    {
        path = EnvironmentUtils::GetEnvironmentVariableValue("USERPROFILE");
    }
    if (StringUtils::Trim(path).empty())
    {
        path = PathUtils::Combine(EnvironmentUtils::GetEnvironmentVariableValue("HOMEDRIVE"), EnvironmentUtils::GetEnvironmentVariableValue("HOMEPATH"));
    }

    return path;
}

string DirectoryUtils::GetUserDataDirectory()
{
    auto path = PathUtils::Combine(GetUserHomeDirectory(), "AppData", "Local");
    return path;
}

bool DirectoryUtils::Exists(const string& path)
{
    error_code error;
    if (!exists(path, error) || error.value() != 0)
        return false;

    return is_directory(path, error) && error.value() == 0;
}

bool DirectoryUtils::Create(const string& path)
{
    if (StringUtils::Trim(path).empty())
        return false;

    if (Exists(path))
        return true;

    const auto parent = StringUtils::BeforeLast(StringUtils::RemoveEndsWith(path, PathUtils::PATH_SEPARATOR), PathUtils::PATH_SEPARATOR);
    if (parent.empty() || StringUtils::EndsWith(parent, PathUtils::DRIVE_SEPARATOR))
        return true;

    if (!Create(parent))
        return false;

    auto _ = _mkdir(path.c_str());

    return true;
}

bool DirectoryUtils::Delete(const string& path, bool recurse_sub_directories, bool remove_files)
{
    // TODO: recursion needs implementing

    if (!Exists(path))
        return false;

    for (const auto& entry : filesystem::directory_iterator(path))
        cout << "DEBUG: " << entry.path() << endl;

    const auto part = StringUtils::BeforeLast(path, PathUtils::PATH_SEPARATOR);
    if (part.empty() || StringUtils::EndsWith(part, PathUtils::DRIVE_SEPARATOR))
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

bool DirectoryUtils::Move(const string& path, const string& newPath, bool overwrite)
{
    if (!Exists(path))
        return false;

    if (Exists(newPath))
    {
        if (!overwrite)
        {
            return false;
        }

        if (!Delete(newPath, true, true))
        {
            return false;
        }
    }

    try
    {
        error_code error;
        filesystem::rename(path, newPath, error);
        return error.value() == 0 && !Exists(path) && Exists(newPath);
    }
    catch ([[maybe_unused]] exception& ex)
    {
        return false;
    }
}

bool DirectoryUtils::Copy(const string& path, const string& newPath, bool overwrite)
{
    if (!Exists(path))
        return false;

    if (Exists(newPath))
    {
        if (!overwrite)
        {
            return false;
        }

        if (!Delete(newPath, true, true))
        {
            return false;
        }
    }

    try
    {
        error_code error;
        auto options = copy_options::recursive;
        if (overwrite)
        {
            options = options | copy_options::overwrite_existing;
        }

        filesystem::copy(path, newPath, options, error);
        return error.value() == 0 && !Exists(path) && Exists(newPath);
    }
    catch ([[maybe_unused]] exception& ex)
    {
        return false;
    }
}

list<string> DirectoryUtils::GetFiles(const string& path, const string& searchPattern, DirectorySearchOption searchOption)
{
    list<string> files;

    // TODO: Implement search pattern matching

    return files;
}

list<string> DirectoryUtils::GetDirectories(const string& path, const string& searchPattern, DirectorySearchOption searchOption)
{
    list<string> directories;

    // TODO: Implement search pattern matching

    return directories;
}

DateTime DirectoryUtils::GetCreationTime(const string& path)
{
    // TODO: Implement
    return DateTime::Now();
}

DateTime DirectoryUtils::GetLastWriteTime(const string& path)
{
    // TODO: Implement
    return DateTime::Now();
}

DateTime DirectoryUtils::GetLastAccessTime(const string& path)
{
    // TODO: Implement
    return DateTime::Now();
}

bool DirectoryUtils::SetCreationTime(const string& path, const DateTime dateTime)
{
    // TODO: Implement
    return false;
}

bool DirectoryUtils::SetLastWriteTime(const string& path, const DateTime dateTime)
{
    // TODO: Implement
    return false;
}

bool DirectoryUtils::SetLastAccessTime(const string& path, const DateTime dateTime)
{
    // TODO: Implement
    return false;
}
