#include "stdafx.h"
#include "DateUtils.h"
#include "DirectoryUtils.h"
#include "EnvironmentUtils.h"
#include "FileUtils.h"
#include "PathUtils.h"
#include "ProcessUtils.h"
#include "StringUtils.h"
#include <string>
#include <filesystem>
#include <regex>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppClangTidyClangDiagnosticNrvo
// ReSharper disable CppUnsignedZeroComparison

using namespace std;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: PathUtils
//--------------------------------------------------------------------------

const string PathUtils::DRIVE_SEPARATOR     = ":";
const string PathUtils::PATH_SEPARATOR      = "\\";
const string PathUtils::EXTENSION_SEPARATOR = ".";

const string PathUtils::RELATIVE_DIRECTORY_CURRENT = ".";
const string PathUtils::RELATIVE_DIRECTORY_PARENT  = "..";
const string PathUtils::RELATIVE_DIRECTORY_ROOT    = "\\";

string PathUtils::FixUpPath(const string& path)
{
    return StringUtils::Replace(
        StringUtils::Replace(
            StringUtils::Trim(path),
            "/",
            PATH_SEPARATOR
        ),
        "\\",
        PATH_SEPARATOR
    );
}

list<string> PathUtils::GetPathParts(const string& path)
{
    auto parts = StringUtils::SplitTextByAny(
        FixUpPath(path),
        PATH_SEPARATOR,
        PATH_SEPARATOR
    );

    return parts;
}

bool PathUtils::IsDriveReference(const string& filePath)
{
    try
    {
        static const regex pattern ("^[A-Za-z]\\:$");

        smatch matches;
        return std::regex_search(filePath, matches, pattern) && !matches.empty();
    }
    catch ([[maybe_unused]] const exception &ex)
    {
        return false;
    }
}

bool PathUtils::IsFile(const string& filePath)
{
    if (filePath.empty() || IsDriveReference(filePath) || DirectoryUtils::Exists(filePath))
        return false;

    if (FileUtils::Exists(filePath))
    {
        return true;
    }

    const auto parts = GetPathParts(filePath);
    const auto& fileName = parts.back();

    return StringUtils::Contains(fileName, EXTENSION_SEPARATOR);
}

bool PathUtils::IsDirectory(const string& filePath)
{
    if (filePath.empty() || IsDriveReference(filePath) || FileUtils::Exists(filePath))
        return false;

    if (DirectoryUtils::Exists(filePath))
    {
        return true;
    }

    const auto parts = GetPathParts(filePath);
    const auto& fileName = parts.back();

    return !StringUtils::Contains(fileName, EXTENSION_SEPARATOR);
}

string PathUtils::GetParentPath(const string& path)
{
    auto parent_path = StringUtils::BeforeLast(
        StringUtils::RemoveEndsWith(path, PATH_SEPARATOR),
        PATH_SEPARATOR
    );

    if (IsDriveReference(parent_path))
    {
        return StringUtils::EnsureEndsWith(parent_path, PATH_SEPARATOR);
    }

    return parent_path;
}

string PathUtils::GetAbsolutePath(const string& path)
{
    auto parts = GetPathParts(path);

    list<string> absolute_parts;
    for(const auto& part : parts)
    {
        if (part.empty())
            continue;
        if (part == RELATIVE_DIRECTORY_CURRENT)
            continue;
        if (part == RELATIVE_DIRECTORY_PARENT)
        {
            if (absolute_parts.empty())
            {
                return "";
            }

            absolute_parts.pop_back();
            continue;
        }

        absolute_parts.push_back(part);
    }

    return StringUtils::JoinText(absolute_parts, PATH_SEPARATOR);
}

string PathUtils::GetDrive(const string& filePath)
{
    if (filePath.empty())
        return "";

    if (IsDriveReference(filePath))
    {
        return filePath;
    }

    const auto drive_separator_pos = filePath.find(DRIVE_SEPARATOR);
    if (drive_separator_pos == string::npos || drive_separator_pos > 1)
        return "";

    return filePath.substr(0, drive_separator_pos + 1);
}

string PathUtils::GetDriveAndPath(const string& filePath)
{
    if (IsDirectory(filePath))
    {
        return StringUtils::EnsureEndsWith(filePath, PATH_SEPARATOR);
    }

    const auto parts = GetPathParts(filePath);
    const auto& fileName = parts.back();

    return StringUtils::RemoveEndsWith(filePath, fileName);
}

string PathUtils::GetFileNameOnly(const string& filePath)
{
    auto fileName = GetFileNameAndExtension(filePath);

    const auto lastFileExtSep = fileName.find_last_of('.');
    if (lastFileExtSep >= 0)
    {
        fileName = fileName.substr(0, lastFileExtSep);
    }

    return fileName;
}

string PathUtils::GetFileNameAndExtension(const string& filePath)
{
    string fileName = filePath;

    const auto lastPathSep = filePath.find_last_of('\\');
    if (lastPathSep >= 0)
    {
        fileName = filePath.substr(lastPathSep + 1);
    }

    return fileName;
}

string PathUtils::ChangeFileExtension(const string& filePath, const string& fileExtension)
{
    string fileName = filePath;

    const auto lastFileExtSep = fileName.find_last_of('.');
    if (lastFileExtSep >= 0)
    {
        fileName = fileName.substr(0, lastFileExtSep);
    }

    if (!fileExtension.empty() && fileExtension.substr(0, 1) != ".")
    {
        fileName += '.';
    }

    fileName += fileExtension;

    return fileName;
}

string PathUtils::Combine(const initializer_list<string> paths)
{
    string result;

    for (auto& path : paths)
    {
        if (result.empty())
        {
            result = StringUtils::RemoveEndsWith(path, PATH_SEPARATOR);
            continue;
        }

        if (path.empty())
            continue;

        result += PATH_SEPARATOR + StringUtils::RemoveEndsWith(path, PATH_SEPARATOR);
    }

    if (IsDriveReference(result))
        result = StringUtils::EnsureEndsWith(result, PATH_SEPARATOR);

    return result;
}

string PathUtils::Combine(const string& path1, const string& path2)
{
    return Combine({ path1, path2 });
}

string PathUtils::Combine(const string& path1, const string& path2, const string& path3)
{
    return Combine({ path1, path2, path3 });
}

string PathUtils::Combine(const string& path1, const string& path2, const string& path3, const string& path4)
{
    return Combine({ path1, path2, path3, path4 });
}

string PathUtils::Combine(const string& path1, const string& path2, const string& path3, const string& path4, const string& path5)
{
    return Combine({ path1, path2, path3, path4, path5 });
}

string PathUtils::Combine(const string& path1, const string& path2, const string& path3, const string& path4, const string& path5, const string& path6)
{
    return Combine({ path1, path2, path3, path4, path5, path6 });
}

string PathUtils::Combine(const string& path1, const string& path2, const string& path3, const string& path4, const string& path5, const string& path6, const string& path7)
{
    return Combine({ path1, path2, path3, path4, path5, path6, path7 });
}

string PathUtils::Combine(const string& path1, const string& path2, const string& path3, const string& path4, const string& path5, const string& path6, const string& path7, const string& path8)
{
    return Combine({ path1, path2, path3, path4, path5, path6, path7, path8 });
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
        path = Combine(DirectoryUtils::GetUserHomeDirectory(), "Temp");
    }

    if (!path.empty())
    {
        if (!DirectoryUtils::Exists(path))
        {
            DirectoryUtils::Create(path);
        }
    }

    return path;
}

string PathUtils::GetTempFileName()
{
    return GetTempFileName("temp");
}

string PathUtils::GetTempFileName(const string& prefix)
{
    return GetTempFileName(prefix, "tmp");
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
    } while (FileUtils::Exists(fileName));

    FileUtils::Create(fileName);

    return fileName;
}
