#pragma once

#include "stdafx.h"
#include <cstdint>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::Utils
{
    //--------------------------------------------------------------------------
    // Class: FileComparer
    //--------------------------------------------------------------------------
    class FileComparer
    {
        static constexpr uint8_t DEFAULT_BINARY_COMPARE_BLOCK_SIZE = static_cast<uint8_t>(2048);

    public:
        static bool CompareTextFiles(const string& file_name_1, const string& file_name_2);
        static bool CompareBinaryFiles(const string& file_name_1, const string& file_name_2, uint8_t buffer_size = DEFAULT_BINARY_COMPARE_BLOCK_SIZE);
    };
}
