#pragma once

#include "stdafx.h"
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::Utils
{
    //--------------------------------------------------------------------------
    // Class: PathUtils
    //--------------------------------------------------------------------------
    class PathUtils
    {
    public:
        static const string DRIVE_SEPARATOR;
        static const string PATH_SEPARATOR;

        static string Combine(const string& path1, const string& path2);
        static string Combine(const string& path1, const string& path2, const string& path3);
        static string Combine(const string& path1, const string& path2, const string& path3, const string& path4);
        static string Combine(const string& path1, const string& path2, const string& path3, const string& path4, const string& path5);
        static string Combine(const string& path1, const string& path2, const string& path3, const string& path4, const string& path5, const string& path6);

        static string GetCurrentDirectory();
        static string ChangeDirectory(const string& path);

        static bool DirectoryExists(const string& path);
        static bool CreateDirectory(const string& path);
        static bool DeleteDirectory(const string& path, bool recurse_sub_directories = false, bool remove_files = false);

        static string GetTempPath();
        static string GetTempFileName(const string& prefix, const string& extension = "tmp");

        static string GetUserHomeDirectory();
        static string GetUserDataDirectory();
    };
}
