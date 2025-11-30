#pragma once

#include "stdafx.h"
#include <list>
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
        static const string EXTENSION_SEPARATOR;

        static const string RELATIVE_DIRECTORY_CURRENT;
        static const string RELATIVE_DIRECTORY_PARENT;
        static const string RELATIVE_DIRECTORY_ROOT;

        static string FixUpPath(const string& path);
        static list<string> GetPathParts(const string& path);

        static bool IsDriveReference(const string& filePath);
        static bool IsFile(const string& filePath);
        static bool IsDirectory(const string& filePath);

        static string GetParentPath(const string& path);
        static string GetAbsolutePath(const string& path);

        static string GetDrive(const string& filePath);
        static string GetDriveAndPath(const string& filePath);
        static string GetFileNameOnly(const string& filePath);
        static string GetFileNameAndExtension(const string& filePath);
        static string ChangeFileExtension(const string& filePath, const string& fileExtension);

        static string Combine(const string& path1, const string& path2);
        static string Combine(const string& path1, const string& path2, const string& path3);
        static string Combine(const string& path1, const string& path2, const string& path3, const string& path4);
        static string Combine(const string& path1, const string& path2, const string& path3, const string& path4, const string& path5);
        static string Combine(const string& path1, const string& path2, const string& path3, const string& path4, const string& path5, const string& path6);
        static string Combine(const string& path1, const string& path2, const string& path3, const string& path4, const string& path5, const string& path6, const string& path7);
        static string Combine(const string& path1, const string& path2, const string& path3, const string& path4, const string& path5, const string& path6, const string& path7, const string& path8);

        static string GetTempPath();
        static string GetTempFileName();
        static string GetTempFileName(const string& prefix);
        static string GetTempFileName(const string& prefix, const string& extension);
    };
}
