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
    // Class: FileUtils
    //--------------------------------------------------------------------------
    class FileUtils
    {
    public:
        static string GetPath(const string& filePath);
        static string GetFileNameOnly(const string& filePath);
        static string GetFileNameAndExtension(const string& filePath);
        static string ChangeFileExtension(const string& filePath, const string& fileExtension);

        static bool FileExists(const string& fileName);
        static bool Create(const string& fileName);
        static bool Delete(const string& fileName, bool ignoreResultCode = false);

        static list<string> ReadLines(const string& fileName);
        static string ReadText(const string& fileName);
        static void WriteLines(const string& fileName, const list<string>& lines);
        static void WriteText(const string& fileName, const string& text);

        static bool CompareTextFiles(const string& fileName1, const string& fileName2);
        static bool CompareBinaryFiles(const string& fileName1, const string& fileName2);
    };
}
