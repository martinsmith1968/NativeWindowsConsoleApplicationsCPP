#pragma once

#include "stdafx.h"
#include <cstdint>
#include <list>
#include <string>
#include <initializer_list>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::Utils
{
    enum class FileSystemItemAttributes : uint32_t
    {
        None = 0,
        // Windows Specific
        Archive = 1 << 0,
        ReadOnly = 1 << 1,
        System = 1 << 2,
        Hidden = 1 << 3,
        Compressed = 1 << 4,
        Directory = 1 << 5,
        ReparsePoint = 1 << 6,
        NotContentIndexed = 1 << 7,
        Offline = 1 << 8,
        // STL Specific
        Block = 1 << 9,
        RegularFile = 1 << 10,
        CharacterFile = 1 << 11,
        Other = 1 << 12,
        Symlink = 1 << 13,
        FIFO = 1 << 14,
        Socket = 1 << 15,
        Junction = 1 << 16,

    };

    inline FileSystemItemAttributes operator|(FileSystemItemAttributes a, FileSystemItemAttributes b)
    {
        return static_cast<FileSystemItemAttributes>(static_cast<int>(a) | static_cast<int>(b));
    }

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

        static bool IsDriveReference(const string& file_path);
        static bool IsFile(const string& file_path);
        static bool IsDirectory(const string& file_path);

        static bool HasDriveReference(const string& file_path);
        static int GetDriveId(const string& file_path);
        static string GetDriveReference(const int drive_id);

        static string GetParentPath(const string& file_path);
        static string GetAbsolutePath(const string& file_path);

        static string GetDrive(const string& file_path);
        static string GetDriveAndPath(const string& file_path);
        static string GetFileNameOnly(const string& file_path);
        static string GetFileNameAndExtension(const string& file_path);
        static string GetFileExtension(const string& file_path);
        static string ChangeFileExtension(const string& file_path, const string& file_extension);

        static string Combine(initializer_list<string> paths);

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
        static string GetTempFileName(const string& prefix, const string& file_extension);
    };
}
