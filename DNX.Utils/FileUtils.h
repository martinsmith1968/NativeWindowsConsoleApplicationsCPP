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
        static list<string> ReadLines(const string& fileName);
        static void WriteLines(const string& fileName, const list<string>& lines);
    };
}
