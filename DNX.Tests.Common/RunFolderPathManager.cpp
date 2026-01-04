#include "stdafx.h"
#include "RunFolderPathManager.h"
#include "../DNX.Utils/DirectoryUtils.h"
#include "../DNX.Utils/PathUtils.h"
#include <filesystem>
#include <iostream>

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl

void RunFolderPathManager::CreateRunFolderPath() const
{
    DirectoryUtils::Create(m_run_folder_path);
}

string RunFolderPathManager::MoveFileToRunFolder(const string& filename, bool verbose) const
{
    const auto targetFileName = PathUtils::Combine(m_run_folder_path, PathUtils::GetFileNameAndExtension(filename));

    if (verbose)
    {
        cout << "Copying: " << filename << endl
             << "     to: " << targetFileName << endl;
    }
    copy_file(filename.c_str(), targetFileName.c_str(), filesystem::copy_options::overwrite_existing);

    return targetFileName;
}

void RunFolderPathManager::Setup() const
{
    if (!m_working_folder_path.empty())
        DirectoryUtils::SetCurrentDirectory(m_working_folder_path);
}

void RunFolderPathManager::Cleanup() const
{
    if (!m_run_folder_path.empty() && DirectoryUtils::Exists(m_run_folder_path))
        DirectoryUtils::Delete(m_run_folder_path, true, true);

    if (!m_initial_folder_path.empty())
        DirectoryUtils::SetCurrentDirectory(m_initial_folder_path);
}

void RunFolderPathManager::SetRunFolderPath(const string& folder_path)
{
    m_run_folder_path = folder_path;
    CreateRunFolderPath();
}

void RunFolderPathManager::SetInitialFolderPath(const string& folder_path)
{
    m_initial_folder_path = folder_path;
}

void RunFolderPathManager::SetWorkingFolderPath(const string& folder_path)
{
    m_working_folder_path = folder_path;
}

string RunFolderPathManager::GetRunFolderPath() const
{
    return m_run_folder_path;
}

string RunFolderPathManager::GetInitialFolderPath() const
{
    return m_initial_folder_path;
}

string RunFolderPathManager::GetWorkingFolderPath() const
{
    return m_working_folder_path;
}
