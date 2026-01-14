#pragma once

#include "AppRelocator.h"
#include "CurrentFolderHistoryManager.h"
#include "RunIdGenerator.h"
#include "TestRunController.h"
#include "gtest/gtest.h"
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticPadded
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppPolymorphicClassWithNonVirtualPublicDestructor
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions

using namespace std;

namespace DNX::Tests::Common
{
    class TestRunRelocatingController :
        public TestRunController
    {
        RunIdGenerator m_run_id_generator;
        AppRelocator m_app_relocator;
        CurrentFolderHistoryManager m_folder_history_manager;

        string m_run_id;
        string m_run_folder_path;
        string m_relocated_executable_filepath;

    public:
        explicit TestRunRelocatingController(testing::UnitTest* unit_test_instance, AppDetails app_details, const string& executable_name, const string& slug_id = "");

        [[nodiscard]] string GetRunId() const { return m_run_id; }
        [[nodiscard]] string GetRunFolderPath() const { return m_run_folder_path; }
        [[nodiscard]] string GetRelocatedExecutableFilePath() const { return m_relocated_executable_filepath; }

        void SetUp() override;
        void TearDown() override;
    };
}
