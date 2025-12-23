#include "stdafx.h"
#include "FileInfo.h"
#include "DateTime.h"
#include "DirectoryInfo.h"
#include "DirectoryUtils.h"
#include "FileUtils.h"
#include "PathUtils.h"
#include "StringUtils.h"
#include <list>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl

using namespace std;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: FileInfo
//--------------------------------------------------------------------------

FileInfo::FileInfo(const string& file_name)
{
    m_full_file_name = PathUtils::FixUpPath(file_name);

    if (m_full_file_name.empty())
        return;

    if (!PathUtils::HasDriveReference(m_full_file_name))
    {
        m_full_file_name = PathUtils::GetDrive(DirectoryUtils::GetCurrentDirectory())
            + m_full_file_name;
    }

    const auto driveAndPath = StringUtils::After(m_full_file_name, PathUtils::DRIVE_SEPARATOR);
    if (!StringUtils::StartsWith(driveAndPath, PathUtils::PATH_SEPARATOR))
    {
        m_full_file_name = PathUtils::GetDriveAndPath(DirectoryUtils::GetCurrentDirectory())
            + PathUtils::PATH_SEPARATOR
            + driveAndPath;
    }

    m_full_file_name = PathUtils::GetAbsolutePath(m_full_file_name);
}

string FileInfo::GetFullName() const
{
    return m_full_file_name;
}

string FileInfo::GetName() const
{
    return PathUtils::GetFileNameAndExtension(m_full_file_name);
}

string FileInfo::GetExtension() const
{
    return PathUtils::GetFileExtension(m_full_file_name);
}

string FileInfo::GetDirectoryName() const
{
    return PathUtils::GetDriveAndPath(m_full_file_name);
}

DirectoryInfo FileInfo::GetDirectory() const
{
    return DirectoryInfo(GetDirectoryName());
}

bool FileInfo::Exists() const
{
    return FileUtils::Exists(m_full_file_name);
}

int FileInfo::GetSize() const
{
    return FileUtils::GetSize(m_full_file_name);
}

FileSystemItemAttributes FileInfo::GetAttributes() const
{
    return FileUtils::GetAttributes(m_full_file_name);
}

DateTime FileInfo::GetCreationTime() const
{
    return FileUtils::GetCreationTime(m_full_file_name);
}

DateTime FileInfo::GetLastWriteTime() const
{
    return FileUtils::GetLastWriteTime(m_full_file_name);
}

DateTime FileInfo::GetLastAccessTime() const
{
    return FileUtils::GetLastAccessTime(m_full_file_name);
}

bool FileInfo::SetCreationTime(const DateTime date_time) const
{
    return FileUtils::SetCreationTime(m_full_file_name, date_time);
}

bool FileInfo::SetLastWriteTime(const DateTime date_time) const
{
    return FileUtils::SetLastWriteTime(m_full_file_name, date_time);
}

bool FileInfo::SetLastAccessTime(const DateTime date_time) const
{
    return FileUtils::SetLastAccessTime(m_full_file_name, date_time);
}

bool FileInfo::Create(bool overwrite) const
{
    return FileUtils::Create(m_full_file_name, overwrite);
}

bool FileInfo::Delete(bool ignore_result_code) const
{
    return FileUtils::Delete(m_full_file_name, ignore_result_code);
}

bool FileInfo::MoveTo(const string& destination, const bool overwrite) const
{
    return FileUtils::Move(m_full_file_name, destination, overwrite);
}

bool FileInfo::CopyTo(const string& destination, const bool overwrite) const
{
    return FileUtils::Copy(m_full_file_name, destination, overwrite);
}

list<string> FileInfo::ReadAllLines() const
{
    return FileUtils::ReadAllLines(m_full_file_name);
}

string FileInfo::ReadAllText() const
{
    return FileUtils::ReadAllText(m_full_file_name);
}

void FileInfo::WriteAllLines(const list<string>& lines) const
{
    return FileUtils::WriteAllLines(m_full_file_name, lines);
}

void FileInfo::WriteAllText(const string& text) const
{
    return FileUtils::WriteAllText(m_full_file_name, text);
}

void FileInfo::AppendAllLines(const list<string>& lines) const
{
    return FileUtils::AppendAllLines(m_full_file_name, lines);
}

void FileInfo::AppendAllText(const string& text) const
{
    return FileUtils::AppendAllText(m_full_file_name, text);
}
