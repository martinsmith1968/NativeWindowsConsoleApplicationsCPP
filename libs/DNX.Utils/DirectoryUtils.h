#pragma once

#include "stdafx.h"
#include "DateTime.h"
#include <cstdint>
#include <list>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::Utils
{
    enum class DirectorySearchOption : uint8_t
    {
        TopDirectoryOnly,
        AllDirectories
    };

    //--------------------------------------------------------------------------
    // Class: DirectoryUtils
    //--------------------------------------------------------------------------
    class DirectoryUtils
    {
    public:
        static string GetCurrentDirectory();
        static string GetCurrentDirectory(const string& drive_reference);
        static string SetCurrentDirectory(const string& path);

        static string GetUserHomeDirectory();
        static string GetUserDataDirectory();

        static bool Exists(const string& path);
        static bool Create(const string& path);
        static bool Delete(const string& path, bool recurse_sub_directories = false, bool remove_files = false);
        static bool Move(const string& path, const string& newPath, bool overwrite = false);
        static bool Copy(const string& path, const string& newPath, bool overwrite = false);

        static list<string> GetFiles(const string& path, const string& searchPattern, DirectorySearchOption searchOption = DirectorySearchOption::TopDirectoryOnly);
        static list<string> GetDirectories(const string& path, const string& searchPattern, DirectorySearchOption searchOption = DirectorySearchOption::TopDirectoryOnly);

        static DateTime GetCreationTime(const string& path);
        static DateTime GetLastWriteTime(const string& path);
        static DateTime GetLastAccessTime(const string& path);
        static bool SetCreationTime(const string& path, DateTime dateTime);
        static bool SetLastWriteTime(const string& path, DateTime dateTime);
        static bool SetLastAccessTime(const string& path, DateTime dateTime);
    };
}
