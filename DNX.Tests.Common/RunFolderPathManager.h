#pragma once

#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::Tests::Common
{
    class RunFolderPathManager
    {
        string m_run_folder_path;
        string m_initial_folder_path;
        string m_working_folder_path;

        void CreateRunFolderPath() const;

    public:
        [[nodiscard]] string MoveFileToRunFolder(const string& filename, bool verbose = true) const;
        void Setup() const;
        void Cleanup() const;

        void SetRunFolderPath(const string& folder_path);
        void SetInitialFolderPath(const string& folder_path);
        void SetWorkingFolderPath(const string& folder_path);

        [[nodiscard]] string GetRunFolderPath() const;
        [[nodiscard]] string GetInitialFolderPath() const;
        [[nodiscard]] string GetWorkingFolderPath() const;
    };
}
