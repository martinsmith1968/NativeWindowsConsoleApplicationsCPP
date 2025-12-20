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

bool PathUtils::IsDriveReference(const string& file_path)
{
    try
    {
        static const regex pattern ("^[A-Za-z]\\:$");

        smatch matches;
        return std::regex_search(file_path, matches, pattern) && !matches.empty();
    }
    catch ([[maybe_unused]] const exception &ex)
    {
        return false;
    }
}

bool PathUtils::IsFile(const string& file_path)
{
    if (file_path.empty() || IsDriveReference(file_path) || DirectoryUtils::Exists(file_path))
        return false;

    if (FileUtils::Exists(file_path))
    {
        return true;
    }

    const auto parts = GetPathParts(file_path);
    const auto& fileName = parts.back();

    return StringUtils::Contains(fileName, EXTENSION_SEPARATOR);
}

bool PathUtils::IsDirectory(const string& file_path)
{
    if (file_path.empty() || IsDriveReference(file_path) || FileUtils::Exists(file_path))
        return false;

    if (DirectoryUtils::Exists(file_path))
    {
        return true;
    }

    const auto parts = GetPathParts(file_path);
    const auto& fileName = parts.back();

    return !StringUtils::Contains(fileName, EXTENSION_SEPARATOR);
}

bool PathUtils::HasDriveReference(const string& file_path)
{
    if (file_path.length() < 2)
        return false;

    const auto drive_part = file_path.substr(0, 2);
    return IsDriveReference(drive_part);
}

int PathUtils::GetDriveId(const string& file_path)
{
    if (HasDriveReference(file_path))
        return 0;

    const auto drive_char = StringUtils::ToUpper(GetDrive(file_path))[0];
    if (drive_char < 'A' || drive_char > 'Z')
        return 0;

    return drive_char - 'A' + 1;
}

string PathUtils::GetDriveReference(const int drive_id)
{
    if ((drive_id < 'A' || drive_id > 'Z') && (drive_id < 'a' || drive_id > 'z'))
        return "";

    return to_string(static_cast<char>(drive_id));
}

string PathUtils::GetParentPath(const string& file_path)
{
    auto parent_path = StringUtils::BeforeLast(
        StringUtils::RemoveEndsWith(file_path, PATH_SEPARATOR),
        PATH_SEPARATOR
    );

    if (IsDriveReference(parent_path))
    {
        return StringUtils::EnsureEndsWith(parent_path, PATH_SEPARATOR);
    }

    return parent_path;
}

string PathUtils::GetAbsolutePath(const string& file_path)
{
    auto parts = GetPathParts(file_path);

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

string PathUtils::GetDrive(const string& file_path)
{
    if (file_path.empty())
        return "";

    if (IsDriveReference(file_path))
    {
        return file_path;
    }

    const auto drive_separator_pos = file_path.find(DRIVE_SEPARATOR);
    if (drive_separator_pos == string::npos || drive_separator_pos > 1)
        return "";

    return file_path.substr(0, drive_separator_pos + 1);
}

string PathUtils::GetDriveAndPath(const string& file_path)
{
    if (IsDirectory(file_path))
    {
        return StringUtils::EnsureEndsWith(file_path, PATH_SEPARATOR);
    }

    const auto parts = GetPathParts(file_path);
    const auto& fileName = parts.back();

    return StringUtils::RemoveEndsWith(file_path, fileName);
}

string PathUtils::GetFileNameOnly(const string& file_path)
{
    auto fileName = GetFileNameAndExtension(file_path);

    const auto lastFileExtSep = fileName.find_last_of('.');
    if (lastFileExtSep >= 0)
    {
        fileName = fileName.substr(0, lastFileExtSep);
    }

    return fileName;
}

string PathUtils::GetFileNameAndExtension(const string& file_path)
{
    string fileName = file_path;

    const auto lastPathSep = file_path.find_last_of('\\');
    if (lastPathSep >= 0)
    {
        fileName = file_path.substr(lastPathSep + 1);
    }

    return fileName;
}

string PathUtils::GetFileExtension(const string& file_path)
{
    return StringUtils::AfterLast(file_path, EXTENSION_SEPARATOR);
}

string PathUtils::ChangeFileExtension(const string& file_path, const string& file_extension)
{
    string fileName = file_path;

    const auto lastFileExtSep = fileName.find_last_of('.');
    if (lastFileExtSep >= 0)
    {
        fileName = fileName.substr(0, lastFileExtSep);
    }

    if (!file_extension.empty() && file_extension.substr(0, 1) != ".")
    {
        fileName += '.';
    }

    fileName += file_extension;

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

string PathUtils::GetTempFileName(const string& prefix, const string& file_extension)
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

        fileName = Combine(filePath, filePrefix + fileId + "." + file_extension);
    } while (FileUtils::Exists(fileName));

    FileUtils::Create(fileName);

    return fileName;
}
