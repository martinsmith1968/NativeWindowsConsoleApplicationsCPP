#pragma once

#include "stdafx.h"

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
    private:
        static time_t ConvertDateTime(const file_time_type date_time);
        static file_time_type ConvertDateTime(const time_t date_time);

    public:
        static bool Exists(const string& fileName);
        static int GetSize(const string& fileName);
        static string GetAttributes(const string& fileName);

        static time_t GetCreationTime(const string& fileName);
        static time_t GetLastWriteTime(const string& fileName);
        static time_t GetLastAccessTime(const string& fileName);
        static bool SetCreationTime(const string& fileName, const time_t& dateTime);
        static bool SetLastWriteTime(const string& fileName, const time_t& dateTime);
        static bool SetLastAccessTime(const string& fileName, const time_t& dateTime);

        static bool Create(const string& fileName);
        static bool Delete(const string& fileName, bool ignoreResultCode = false);
        static bool Move(const string& fileName, const string& newFileName, bool overwrite = false);
        static bool Copy(const string& fileName, const string& newFileName, bool overwrite = false);

        static list<string> ReadAllLines(const string& fileName);
        static string ReadAllText(const string& fileName);
        static void WriteAllLines(const string& fileName, const list<string>& lines);
        static void WriteAllText(const string& fileName, const string& text);
        static void AppendAllLines(const string& fileName, const list<string>& lines);
        static void AppendAllText(const string& fileName, const string& text);

        static bool CompareTextFiles(const string& fileName1, const string& fileName2);
        static bool CompareBinaryFiles(const string& fileName1, const string& fileName2);
    };
}
