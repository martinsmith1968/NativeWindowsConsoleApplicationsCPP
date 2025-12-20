#include "stdafx.h"
#include "DateTime.h"
#include "FileUtils.h"
#include "PathUtils.h"
#include <chrono>
#include <sys/stat.h>
#include <exception>
#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <list>
#include <ios>
#include <string>
#include <system_error>

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

DateTime FileUtils::ConvertToDateTime(const file_time_type file_time)
{
    const auto result = file_time.time_since_epoch().count();

    return DateTime(result);
}

file_time_type FileUtils::ConvertToFileTime(DateTime date_time)
{
    const auto time_since_epoch = chrono::system_clock::from_time_t(date_time.GetAsTimeT()).time_since_epoch();

    const auto result = file_time_type(time_since_epoch);

    return result;
}

bool FileUtils::Exists(const string& file_name)
{
    error_code error;
    if (!exists(file_name, error) || error.value() != 0)
        return false;

    return is_regular_file(file_name, error) && error.value() == 0;
}

int FileUtils::GetSize(const string& file_name)
{
    error_code error;
    const auto result = file_size(file_name, error);

    if (error.value() != 0)
    {
        return -1;
    }

    return static_cast<int>(result);
}

FileSystemItemAttributes FileUtils::GetAttributes(const string& file_name)
{
    // TODO: Convert to C++ 17 standard
    // NOTE: Convert when C++ 20 is available : https://www.cppstories.com/2024/file-time-cpp20

    auto attributes = FileSystemItemAttributes::None;

    if (Exists(file_name))
    {
        error_code error;
        const auto file_status = status(file_name, error);
        if (error.value() != 0)
            return attributes;

        switch (file_status.type()) {
        case file_type::none:
        case file_type::not_found:
        case file_type::unknown:
            attributes = FileSystemItemAttributes::None;
            break;
        case file_type::regular:
            attributes = attributes | FileSystemItemAttributes::CharacterFile;
            break;
        case file_type::directory:
            attributes = attributes | FileSystemItemAttributes::Directory;
            break;
        case file_type::symlink:
            attributes = attributes | FileSystemItemAttributes::Symlink;
            break;
        case file_type::block:
            attributes = attributes | FileSystemItemAttributes::Block;
            break;
        case file_type::character:
            attributes = attributes | FileSystemItemAttributes::CharacterFile;
            break;
        case file_type::fifo:
            attributes = attributes | FileSystemItemAttributes::FIFO;
            break;
        case file_type::socket:
            attributes = attributes | FileSystemItemAttributes::Socket;
            break;
        case file_type::junction:
            attributes = attributes | FileSystemItemAttributes::Junction;
            break;
        }
    }

    return attributes;
}

void FileUtils::SetAttributes(const string& file_name, FileSystemItemAttributes attributes)
{




    struct stat status;



    stat(file_name.c_str(), &status);
}

DateTime FileUtils::GetCreationTime(const string& file_name)
{
    // TODO: Convert to C++ 17 standard
    // NOTE: Convert when C++ 20 is available : https://www.cppstories.com/2024/file-time-cpp20

    struct stat status_buffer;
    stat(file_name.c_str(), &status_buffer);

    return DateTime(status_buffer.st_ctime);
}

DateTime FileUtils::GetLastWriteTime(const string& file_name)
{
    // TODO: Convert to C++ 17 standard
    // NOTE: Convert when C++ 20 is available : https://www.cppstories.com/2024/file-time-cpp20
    struct stat status_buffer;
    stat(file_name.c_str(), &status_buffer);

    return DateTime(status_buffer.st_mtime);
}

DateTime FileUtils::GetLastAccessTime(const string& file_name)
{
    // TODO: Convert to C++ 17 standard
    // NOTE: Convert when C++ 20 is available : https://www.cppstories.com/2024/file-time-cpp20
    struct stat status_buffer;
    stat(file_name.c_str(), &status_buffer);

    return DateTime(status_buffer.st_atime);
}

bool FileUtils::SetCreationTime(const string& file_name, DateTime date_time)
{
    return false; // TODO: Creation time is not supported in C++17 filesystem library
}

bool FileUtils::SetLastWriteTime(const string& file_name, DateTime date_time)
{
    error_code error;
    last_write_time(file_name, ConvertToFileTime(date_time), error);

    return error.value() == 0;
}

bool FileUtils::SetLastAccessTime(const string& file_name, DateTime date_time)
{
    // Not Supported

    return false;
}

bool FileUtils::Create(const string& file_name, const bool overwrite)
{
    if (!overwrite && Exists(file_name))
    {
        return false;
    }

    auto file = ofstream{ file_name };
    file.close();

    return Exists(file_name);
}

bool FileUtils::Delete(const string& file_name, const bool ignore_result_code)
{
    error_code error;
    const auto result = filesystem::remove(file_name, error);

    return (ignore_result_code || (result && error.value() == 0)) && !Exists(file_name);
}

bool FileUtils::Move(const string& file_name, const string& destination, const bool overwrite)
{
    try
    {
        if (overwrite && Exists(destination))
        {
            if (!Delete(destination, true))
            {
                return false;
            }
        }

        error_code error;
        filesystem::rename(file_name, destination, error);

        return error.value() == 0 && !Exists(file_name) && Exists(destination);
    }
    catch ([[maybe_unused]] exception& ex)
    {
        return false;
    }
}

bool FileUtils::Copy(const string& file_name, const string& destination, const bool overwrite)
{
    if (!Exists(file_name))
        return false;

    copy_options options = overwrite
        ? copy_options::overwrite_existing
        : copy_options::none;

    try
    {
        error_code error;
        copy_file(file_name, destination, options, error);

        return error.value() == 0;
    }
    catch ([[maybe_unused]] exception& ex)
    {
        return false;
    }
}

list<string> FileUtils::ReadAllLines(const string& file_name)
{
    list<string> lines;

    if (Exists(file_name))
    {
        try
        {
            if (ifstream in(file_name); in)
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

string FileUtils::ReadAllText(const string& file_name)
{
    stringstream text;

    if (Exists(file_name))
    {
        if (ifstream in(file_name); in)
        {
            text << in.rdbuf();

            in.close();
        }
    }

    return text.str();
}

void FileUtils::WriteAllLines(const string& file_name, const list<string>& lines)
{
    if (ofstream out(file_name); out)
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

void FileUtils::WriteAllText(const string& file_name, const string& text)
{
    if (ofstream out(file_name); out)
    {
        if (out.is_open())
        {
            out << text;
            out.close();
        }
    }
}

void FileUtils::AppendAllLines(const string& file_name, const list<string>& lines)
{
    if (ofstream out(file_name, ios_base::ate); out)
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

void FileUtils::AppendAllText(const string& file_name, const string& text)
{
    if (ofstream out(file_name, ios_base::ate); out)
    {
        if (out.is_open())
        {
            out << text;
            out.close();
        }
    }
}
