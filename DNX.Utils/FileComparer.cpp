#include "stdafx.h"
#include "FileComparer.h"
#include "FileUtils.h"
#include <filesystem>
#include <fstream>
#include <iostream>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl

using namespace std;
using namespace DNX::Utils;

bool FileComparer::CompareTextFiles(const string& file_name_1, const string& file_name_2)
{
    if (!FileUtils::Exists(file_name_1))
        return false;
    if (!FileUtils::Exists(file_name_2))
        return false;

    if (FileUtils::GetSize(file_name_1) != FileUtils::GetSize(file_name_2))
        return false;

    const auto content1 = FileUtils::ReadAllLines(file_name_1);
    const auto content2 = FileUtils::ReadAllLines(file_name_2);

    return content1.size() == content2.size()
        && content1 == content2;
}

bool FileComparer::CompareBinaryFiles(const string& file_name_1, const string& file_name_2, const uint8_t buffer_size)
{
    if (!FileUtils::Exists(file_name_1))
        return false;
    if (!FileUtils::Exists(file_name_2))
        return false;

    if (FileUtils::GetSize(file_name_1) != FileUtils::GetSize(file_name_2))
        return false;

    const auto buffer1 = new char[buffer_size];
    const auto buffer2 = new char[buffer_size];

    fstream file1(file_name_1, ios::in | ios::binary);
    fstream file2(file_name_2, ios::in | ios::binary);

    auto files_equal = true;
    while (!file1.eof() && !file2.eof() && files_equal)
    {
        for (auto i = 0; i < buffer_size; ++i)
        {
            buffer1[i] = '0';
            buffer2[i] = '0';
        }

        file1.read(buffer1, buffer_size);
        file2.read(buffer1, buffer_size);

        for (auto i = 0; i < buffer_size; ++i)
        {
            if (buffer1[i] != buffer2[i])
            {
                files_equal = false;
                break;
            }
        }
    }

    file1.close();
    file2.close();

    return files_equal;
}
