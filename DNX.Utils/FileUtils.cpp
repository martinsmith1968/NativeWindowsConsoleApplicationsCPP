#include "stdafx.h"
#include "FileUtils.h"
#include <fstream>
#include <io.h>

#include "PathUtils.h"
#include "StringUtils.h"

#define ACCESS    _access_s

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl

using namespace std;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: FileUtils
//--------------------------------------------------------------------------

string FileUtils::GetPath(const string& filePath)
{
    return StringUtils::BeforeLast(filePath, PathUtils::PATH_SEPARATOR);
}


string FileUtils::GetFileNameOnly(const string& filePath)
{
    auto fileName = GetFileNameAndExtension(filePath);

    const auto lastFileExtSep = fileName.find_last_of('.');
    if (lastFileExtSep >= 0)
    {
        fileName = fileName.substr(0, lastFileExtSep);
    }

    return fileName;
}

string FileUtils::GetFileNameAndExtension(const string& filePath)
{
    auto fileName = filePath;

    const auto lastPathSep = filePath.find_last_of('\\');
    if (lastPathSep >= 0)
    {
        fileName = filePath.substr(lastPathSep + 1);
    }

    return fileName;
}

string FileUtils::ChangeFileExtension(const string& filePath, const string& fileExtension)
{
    auto fileName = filePath;

    const auto lastFileExtSep = fileName.find_last_of('.');
    if (lastFileExtSep >= 0)
    {
        fileName = fileName.substr(0, lastFileExtSep);
    }

    if (!fileExtension.empty() && fileExtension.substr(0, 1) != ".")
    {
        fileName += '.';
    }

    fileName += fileExtension;

    return fileName;
}

bool FileUtils::FileExists(const string& fileName)
{
    return ACCESS(fileName.c_str(), 0) == 0;
}

bool FileUtils::Create(const string& fileName)
{
    auto file = ofstream{ fileName };

    return FileExists(fileName);
}

bool FileUtils::Delete(const string& fileName)
{
    remove(fileName.c_str());

    return !FileExists(fileName);
}

list<string> FileUtils::ReadLines(const string& fileName)
{
    list<string> lines;

    if (FileExists(fileName))
    {
        try
        {
            if (ifstream in(fileName); in)
            {
                string line;

                while (std::getline(in, line))
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

void FileUtils::WriteLines(const string& fileName, const list<string>& lines)
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

bool FileUtils::CompareTextFiles(const string& fileName1, const string& fileName2)
{
    if (!FileExists(fileName1))
        return false;
    if (!FileExists(fileName2))
        return false;

    const auto content1 = ReadLines(fileName1);
    const auto content2 = ReadLines(fileName2);

    return content1.size() == content2.size()
        && content1 == content2;
}

bool FileUtils::CompareBinaryFiles(const string& fileName1, const string& fileName2)
{
    if (!FileExists(fileName1))
        return false;
    if (!FileExists(fileName2))
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
