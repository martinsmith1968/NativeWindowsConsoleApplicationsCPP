#pragma once

#include "stdafx.h"
#include <stack>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions

using namespace std;

namespace DNX::Tests::Common
{
    class CurrentFolderHistoryManager
    {
        stack<string> m_folder_history;

    public:
        CurrentFolderHistoryManager() = default;
        ~CurrentFolderHistoryManager();

        void Cleanup();

        void ChangeCurrentFolderTo(const string& folder_path);
    };
}
