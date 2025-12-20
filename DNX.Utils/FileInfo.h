#pragma once

#include "stdafx.h"
#include "DateTime.h"
#include "PathUtils.h"
#include "DirectoryInfo.h"
#include <list>
#include <string>

// ReSharper disable CppInconsistentNaming

namespace DNX::Utils
{
    //--------------------------------------------------------------------------
    // Class: FileInfo
    //--------------------------------------------------------------------------
    class FileInfo
    {
        string m_full_file_name;

    public:
        explicit FileInfo(const string& file_name);

        [[nodiscard]] string GetFullName() const;
        [[nodiscard]] string GetName() const;
        [[nodiscard]] string GetExtension() const;

        [[nodiscard]] string GetDirectoryName() const;
        [[nodiscard]] DirectoryInfo GetDirectory() const;

        [[nodiscard]] bool Exists() const;
        [[nodiscard]] int GetSize() const;
        [[nodiscard]] FileSystemItemAttributes GetAttributes() const;

        [[nodiscard]] DateTime GetCreationTime() const;
        [[nodiscard]] DateTime GetLastWriteTime() const;
        [[nodiscard]] DateTime GetLastAccessTime() const;
        bool SetCreationTime(DateTime date_time) const;
        bool SetLastWriteTime(DateTime date_time) const;
        bool SetLastAccessTime(DateTime date_time) const;

        bool Create(bool overwrite = false) const;
        bool Delete(bool ignore_result_code = false) const;
        bool MoveTo(const string& destination, bool overwrite = false) const;
        [[nodiscard]] bool CopyTo(const string& destination, bool overwrite = false) const;

        [[nodiscard]] list<string> ReadAllLines() const;
        [[nodiscard]] string ReadAllText() const;
        void WriteAllLines(const list<string>& lines) const;
        void WriteAllText(const string& text) const;
        void AppendAllLines(const list<string>& lines) const;
        void AppendAllText(const string& text) const;
    };
}
