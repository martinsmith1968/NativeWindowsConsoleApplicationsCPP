#include "stdafx.h"
#include "CurrentFolderHistoryManager.h"
#include "../DNX.Utils/DirectoryUtils.h"
#include <filesystem>

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl

CurrentFolderHistoryManager::~CurrentFolderHistoryManager()
{
    Cleanup();
}

void CurrentFolderHistoryManager::Cleanup()
{
    while (!m_folder_history.empty())
    {
        DirectoryUtils::SetCurrentDirectory(m_folder_history.top());
        m_folder_history.pop();
    }
}

void CurrentFolderHistoryManager::ChangeCurrentFolderTo(const string& folder_path)
{
    m_folder_history.push(DirectoryUtils::GetCurrentDirectory());
    DirectoryUtils::SetCurrentDirectory(folder_path);
}
