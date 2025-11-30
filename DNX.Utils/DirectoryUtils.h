#pragma once

#include "stdafx.h"
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
        static string SetCurrentDirectory(const string& path);

        static string GetUserHomeDirectory();
        static string GetUserDataDirectory();

        static bool Exists(const string& path);
        static bool Create(const string& path);
        static bool Delete(const string& path, bool recurse_sub_directories = false, bool remove_files = false);
        static bool Move(const string& path, const string& newPath);

        static list<string> GetFiles(const string& path, const string& searchPattern, DirectorySearchOption searchOption = DirectorySearchOption::TopDirectoryOnly);
        static list<string> GetDirectories(const string& path, const string& searchPattern, DirectorySearchOption searchOption = DirectorySearchOption::TopDirectoryOnly);

        static time_t GetCreationTime(const string& path);
        static time_t GetLastWriteTime(const string& path);
        static time_t GetLastAccessTime(const string& path);
        static void SetCreationTime(const string& path, const time_t& dateTime);
        static void SetLastWriteTime(const string& path, const time_t& dateTime);
        static void SetLastAccessTime(const string& path, const time_t& dateTime);
    };
}
