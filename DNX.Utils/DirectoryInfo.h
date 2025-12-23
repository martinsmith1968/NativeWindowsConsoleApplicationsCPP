#pragma once

#include "stdafx.h"
#include "DateTime.h"
#include "PathUtils.h"
#include <string>

// ReSharper disable CppInconsistentNaming

namespace DNX::Utils
{
    //--------------------------------------------------------------------------
    // Class: DirectoryInfo
    //--------------------------------------------------------------------------
    class DirectoryInfo
    {
        string m_full_directory_name;

    public:
        explicit DirectoryInfo(const string& directory_name);

        [[nodiscard]] string GetFullName() const;
        [[nodiscard]] string GetName() const;
        [[nodiscard]] string GetExtension() const;

        [[nodiscard]] DirectoryInfo GetParentDirectory() const;
        [[nodiscard]] DirectoryInfo GetRootDirectory() const;

        [[nodiscard]] bool Exists() const;
        [[nodiscard]] FileSystemItemAttributes GetAttributes() const;
        [[nodiscard]] bool IsRoot() const;

        [[nodiscard]] DateTime GetCreationTime() const;
        [[nodiscard]] DateTime GetLastWriteTime() const;
        [[nodiscard]] DateTime GetLastAccessTime() const;
        bool SetCreationTime(DateTime date_time) const;
        bool SetLastWriteTime(DateTime date_time) const;
        bool SetLastAccessTime(DateTime date_time) const;

        bool Create() const;
        [[nodiscard]] bool Delete(bool recurse_sub_directories = false, bool remove_files = false) const;
        [[nodiscard]] bool MoveTo(const string& destination, bool overwrite = false) const;
        [[nodiscard]] bool CopyTo(const string& destination, bool overwrite = false) const;
    };
}
