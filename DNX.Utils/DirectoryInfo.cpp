#include "stdafx.h"
#include "DateTime.h"
#include "DirectoryInfo.h"
#include "DirectoryUtils.h"
#include "PathUtils.h"
#include "StringUtils.h"
#include <string>

#include "FileUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppMemberFunctionMayBeConst

using namespace std;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: DirectoryInfo
//--------------------------------------------------------------------------

DirectoryInfo::DirectoryInfo(const string& directory_name)
{
    m_full_directory_name = PathUtils::FixUpPath(directory_name);

    if (m_full_directory_name.empty())
        return;

    if (!PathUtils::HasDriveReference(m_full_directory_name))
    {
        m_full_directory_name = PathUtils::GetDrive(DirectoryUtils::GetCurrentDirectory())
            + m_full_directory_name;
    }

    const auto driveAndPath = StringUtils::After(m_full_directory_name, PathUtils::DRIVE_SEPARATOR);
    if (!StringUtils::StartsWith(driveAndPath, PathUtils::PATH_SEPARATOR))
    {
        m_full_directory_name = PathUtils::GetDriveAndPath(DirectoryUtils::GetCurrentDirectory())
            + PathUtils::PATH_SEPARATOR
            + driveAndPath;
    }

    m_full_directory_name = PathUtils::GetAbsolutePath(m_full_directory_name);
}

string DirectoryInfo::GetFullName() const
{
    return m_full_directory_name;
}

string DirectoryInfo::GetName() const
{
    return PathUtils::GetFileNameAndExtension(m_full_directory_name);
}

string DirectoryInfo::GetExtension() const
{
    return PathUtils::GetFileExtension(m_full_directory_name);
}

DirectoryInfo DirectoryInfo::GetParentDirectory() const
{
    if (IsRoot())
        return DirectoryInfo("");

    const auto parent_path = PathUtils::GetParentPath(m_full_directory_name);

    return DirectoryInfo(parent_path);
}

DirectoryInfo DirectoryInfo::GetRootDirectory() const
{
    const auto root_path = PathUtils::GetDrive(m_full_directory_name)
        + PathUtils::PATH_SEPARATOR;

    return DirectoryInfo(root_path);
}

bool DirectoryInfo::Exists() const
{
    return DirectoryUtils::Exists(m_full_directory_name);
}

FileSystemItemAttributes DirectoryInfo::GetAttributes() const
{
    return FileUtils::GetAttributes(m_full_directory_name);
}

bool DirectoryInfo::IsRoot() const
{
    const auto drive_reference = PathUtils::GetDrive(m_full_directory_name);

    return (m_full_directory_name == drive_reference + PathUtils::PATH_SEPARATOR);
}

DateTime DirectoryInfo::GetCreationTime() const
{
    return DirectoryUtils::GetCreationTime(m_full_directory_name);
}

DateTime DirectoryInfo::GetLastWriteTime() const
{
    return DirectoryUtils::GetLastWriteTime(m_full_directory_name);
}

DateTime DirectoryInfo::GetLastAccessTime() const
{
    return DirectoryUtils::GetLastAccessTime(m_full_directory_name);
}

bool DirectoryInfo::SetCreationTime(const DateTime date_time) const
{
    return DirectoryUtils::SetCreationTime(m_full_directory_name, date_time);
}

bool DirectoryInfo::SetLastWriteTime(const DateTime date_time) const
{
    return DirectoryUtils::SetLastWriteTime(m_full_directory_name, date_time);
}

bool DirectoryInfo::SetLastAccessTime(const DateTime date_time) const
{
    return DirectoryUtils::SetLastAccessTime(m_full_directory_name, date_time);
}

bool DirectoryInfo::Create() const
{
    return DirectoryUtils::Create(m_full_directory_name);
}

bool DirectoryInfo::Delete(const bool recurse_sub_directories, const bool remove_files) const
{
    return DirectoryUtils::Delete(m_full_directory_name, recurse_sub_directories, remove_files);
}

bool DirectoryInfo::MoveTo(const string& destination, const bool overwrite) const
{
    return DirectoryUtils::Move(m_full_directory_name, destination, overwrite);
}

bool DirectoryInfo::CopyTo(const string& destination, const bool overwrite) const
{
    return DirectoryUtils::Copy(m_full_directory_name, destination, overwrite);
}
