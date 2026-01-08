#include "stdafx.h"
#include "AppRelocator.h"
#include "../DNX.Utils/DirectoryUtils.h"
#include "../DNX.Utils/PathUtils.h"
#include <filesystem>
#include <iostream>

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl

void AppRelocator::CreateAppFolderPath() const
{
    if (!m_app_folder_path.empty())
        DirectoryUtils::Create(m_app_folder_path);
}

AppRelocator::~AppRelocator()
{
    Cleanup();
}

void AppRelocator::Cleanup() const
{
    if (!m_app_folder_path.empty() && DirectoryUtils::Exists(m_app_folder_path))
        DirectoryUtils::Delete(m_app_folder_path, true, true);
}

string AppRelocator::CopyFileToAppFolder(const string& filename, const bool verbose) const
{
    const auto targetFileName = PathUtils::Combine(m_app_folder_path, PathUtils::GetFileNameAndExtension(filename));

    if (verbose)
    {
        cout << "Copying: " << filename << endl
             << "     to: " << targetFileName << endl;
    }
    copy_file(filename.c_str(), targetFileName.c_str(), filesystem::copy_options::overwrite_existing);

    return targetFileName;
}

void AppRelocator::SetAppFolderPath(const string& folder_path)
{
    m_app_folder_path = folder_path;
    CreateAppFolderPath();
}

string AppRelocator::GetAppFolderPath() const
{
    return m_app_folder_path;
}
