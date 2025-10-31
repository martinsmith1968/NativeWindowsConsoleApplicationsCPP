#include "stdafx.h"
#include "FileUtils.h"
#include "PathUtils.h"
#include <chrono>
#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <io.h>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl

using namespace std;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: FileUtils
//--------------------------------------------------------------------------
// Refactored to use C++17 filesystem library : https://en.cppreference.com/w/cpp/filesystem.html

// Useful Sources
// https://stackoverflow.com/questions/61030383/how-to-convert-stdfilesystemfile-time-type-to-time-t

time_t FileUtils::ConvertDateTime(const file_time_type date_time)
{
    const auto result = date_time.time_since_epoch().count();

    return result < 0
        ? 0
        : result;
}

file_time_type FileUtils::ConvertDateTime(const time_t date_time)
{
    //const file_time_type result = file_time_type<chrono::system_clock>(
    //    chrono::duration_cast<chrono::system_clock::duration>(
    //        chrono::seconds()
    //    )
    //);

    file_time_type result;

    return result;
}

bool FileUtils::Exists(const string& fileName)
{
    error_code error;
    if (!exists(fileName, error) || error.value() != 0)
        return false;

    return is_regular_file(fileName, error) && error.value() == 0;
}

int FileUtils::GetSize(const string& fileName)
{
    error_code error;
    const auto result = file_size(fileName, error);

    if (error.value() != 0)
    {
        return -1;
    }

    return static_cast<int>(result);
}

string FileUtils::GetAttributes(const string& fileName)
{
    // TODO: Convert to C++ 17 standard
    // NOTE: Convert when C++ 20 is available : https://www.cppstories.com/2024/file-time-cpp20
    struct stat status_buffer;
    stat(fileName.c_str(), &status_buffer);

    error_code error;
    auto result = status(fileName, error);

    return ""; // TODO : Need something other than string here
}

time_t FileUtils::GetCreationTime(const string& fileName)
{
    // TODO: Convert to C++ 17 standard
    // NOTE: Convert when C++ 20 is available : https://www.cppstories.com/2024/file-time-cpp20
    struct stat status_buffer;
    stat(fileName.c_str(), &status_buffer);

    return status_buffer.st_ctime;
}

time_t FileUtils::GetLastWriteTime(const string& fileName)
{
    // TODO: Convert to C++ 17 standard
    // NOTE: Convert when C++ 20 is available : https://www.cppstories.com/2024/file-time-cpp20
    struct stat status_buffer;
    stat(fileName.c_str(), &status_buffer);

    return status_buffer.st_mtime;
}

time_t FileUtils::GetLastAccessTime(const string& fileName)
{
    // TODO: Convert to C++ 17 standard
    // NOTE: Convert when C++ 20 is available : https://www.cppstories.com/2024/file-time-cpp20
    struct stat status_buffer;
    stat(fileName.c_str(), &status_buffer);

    return status_buffer.st_atime;
}

bool FileUtils::SetCreationTime(const string& fileName, const time_t& dateTime)
{
    return false; // TODO: Creation time is not supported in C++17 filesystem library
}

bool FileUtils::SetLastWriteTime(const string& fileName, const time_t& dateTime)
{
    const auto new_time = ConvertDateTime(dateTime);

    error_code error;
    last_write_time(fileName, new_time, error);

    return error.value() == 0;
}

bool FileUtils::SetLastAccessTime(const string& fileName, const time_t& dateTime)
{
    return false; // TODO: Last Access time is not supported in C++17 filesystem library
}

bool FileUtils::Create(const string& fileName)
{
    auto file = ofstream{ fileName };

    return Exists(fileName);
}

bool FileUtils::Delete(const string& fileName, const bool ignoreResultCode)
{
    error_code error;
    const auto result = filesystem::remove(fileName, error);

    return (ignoreResultCode || (result && error.value() == 0)) && !Exists(fileName);
}

bool FileUtils::Move(const string& fileName, const string& newFileName, const bool overwrite)
{
    try
    {
        if (overwrite && Exists(newFileName))
        {
            if (!Delete(newFileName, true))
            {
                return false;
            }
        }

        error_code error;
        filesystem::rename(fileName, newFileName, error);

        return error.value() == 0 && !Exists(fileName) && Exists(newFileName);
    }
    catch ([[maybe_unused]] exception& ex)
    {
        return false;
    }
}

bool FileUtils::Copy(const string& fileName, const string& newFileName, const bool overwrite)
{
    if (!Exists(fileName))
        return false;

    copy_options options = overwrite
        ? copy_options::overwrite_existing
        : copy_options::none;

    try
    {
        error_code error;
        copy_file(fileName, newFileName, options, error);

        return error.value() == 0;
    }
    catch ([[maybe_unused]] exception& ex)
    {
        return false;
    }
}

list<string> FileUtils::ReadAllLines(const string& fileName)
{
    list<string> lines;

    if (Exists(fileName))
    {
        try
        {
            if (ifstream in(fileName); in)
            {
                string line;

                while (getline(in, line))
                {
                    lines.push_back(line);
                }

                in.close();
            }
        }
        catch (exception&)
        {
            lines.clear();
            throw;
        }
    }

    return lines;
}

string FileUtils::ReadAllText(const string& fileName)
{
    stringstream text;

    if (Exists(fileName))
    {
        if (ifstream in(fileName); in)
        {
            text << in.rdbuf();

            in.close();
        }
    }

    return text.str();
}

void FileUtils::WriteAllLines(const string& fileName, const list<string>& lines)
{
    if (ofstream out(fileName); out)
    {
        if (out.is_open())
        {
            for (const string& line : lines)
            {
                out << line << endl;
            }
            out.close();
        }
    }
}

void FileUtils::WriteAllText(const string& fileName, const string& text)
{
    if (ofstream out(fileName); out)
    {
        if (out.is_open())
        {
            out << text;
            out.close();
        }
    }
}

void FileUtils::AppendAllLines(const string& fileName, const list<string>& lines)
{
    if (ofstream out(fileName, ios_base::ate); out)
    {
        if (out.is_open())
        {
            for (const string& line : lines)
            {
                out << line << endl;
            }
            out.close();
        }
    }

}

void FileUtils::AppendAllText(const string& fileName, const string& text)
{
    if (ofstream out(fileName, ios_base::ate); out)
    {
        if (out.is_open())
        {
            out << text;
            out.close();
        }
    }
}

bool FileUtils::CompareTextFiles(const string& fileName1, const string& fileName2)
{
    if (!Exists(fileName1))
        return false;
    if (!Exists(fileName2))
        return false;

    const auto content1 = ReadAllLines(fileName1);
    const auto content2 = ReadAllLines(fileName2);

    return content1.size() == content2.size()
        && content1 == content2;
}

bool FileUtils::CompareBinaryFiles(const string& fileName1, const string& fileName2)
{
    if (!Exists(fileName1))
        return false;
    if (!Exists(fileName2))
        return false;

    constexpr int buffer_size = 2048;

    const auto buffer1 = new char[buffer_size];
    const auto buffer2 = new char[buffer_size];

    fstream file1(fileName1, ios::in | ios::binary);
    fstream file2(fileName2, ios::in | ios::binary);

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
