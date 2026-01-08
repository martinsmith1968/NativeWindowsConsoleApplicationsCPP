#pragma once

#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions

using namespace std;

namespace DNX::Tests::Common
{
    class AppRelocator
    {
        string m_app_folder_path;

        void CreateAppFolderPath() const;

    public:
        AppRelocator() = default;
        ~AppRelocator();
        void Cleanup() const;

        [[nodiscard]] string CopyFileToAppFolder(const string& filename, bool verbose = true) const;

        void SetAppFolderPath(const string& folder_path);
        [[nodiscard]] string GetAppFolderPath() const;
    };
}
