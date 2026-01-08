#pragma once

#include "stdafx.h"
#include "DateTime.h"
#include "PathUtils.h"
#include <filesystem>
#include <list>
#include <string>


// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;
using namespace std::filesystem;

namespace DNX::Utils
{
    //--------------------------------------------------------------------------
    // Class: FileUtils
    //--------------------------------------------------------------------------
    class FileUtils
    {
        static DateTime ConvertToDateTime(const file_time_type file_time);
        static file_time_type ConvertToFileTime(DateTime date_time);

    public:
        static bool Exists(const string& file_name);
        static int GetSize(const string& file_name);
        static FileSystemItemAttributes GetAttributes(const string& file_name);
        static void SetAttributes(const string& file_name, FileSystemItemAttributes attributes);

        static DateTime GetCreationTime(const string& file_name);
        static DateTime GetLastWriteTime(const string& file_name);
        static DateTime GetLastAccessTime(const string& file_name);
        static bool SetCreationTime(const string& file_name, DateTime date_time);
        static bool SetLastWriteTime(const string& file_name, DateTime date_time);
        static bool SetLastAccessTime(const string& file_name, DateTime date_time);

        static bool Create(const string& file_name, bool overwrite = false);
        static bool Delete(const string& file_name, bool ignore_result_code = false);
        static bool Move(const string& file_name, const string& destination, bool overwrite = false);
        static bool Copy(const string& file_name, const string& destination, bool overwrite = false);

        static list<string> ReadAllLines(const string& file_name);
        static string ReadAllText(const string& file_name);
        static void WriteAllLines(const string& file_name, const list<string>& lines);
        static void WriteAllText(const string& file_name, const string& text);
        static void AppendAllLines(const string& file_name, const list<string>& lines);
        static void AppendAllText(const string& file_name, const string& text);
    };
}
