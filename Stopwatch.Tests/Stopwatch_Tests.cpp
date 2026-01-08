#include "pch.h"

#define VER_PRODUCTNAME_STR         "Stopwatch.Tests"
#define VER_FILE_DESCRIPTION_STR    "Stopwatch Google Tests"

#include "../Common/AppInfo.h"
#include "../DNX.Tests.Common/TestHelper.h"
#include "../DNX.Tests.Common/TestRunRelocatingController.h"
#include "../DNX.Utils/DateUtils.h"
#include "../DNX.Utils/DirectoryUtils.h"
#include "gtest/gtest.h"
#include <filesystem>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable StringLiteralTypo

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;
using namespace filesystem;

#define TEST_GROUP Stopwatch

//------------------------------------------------------------------------------

class TEST_GROUP : public testing::Test
{
protected:
    AppInfo m_app_info;
    RunIdGenerator m_run_id_generator;
    RunFolderPathManager m_run_folder_path_manager;
    TestConfig m_test_config = TestConfig(::testing::UnitTest::GetInstance(), "Stopwatch.exe", "sw");

    string m_run_folder_path;
    string m_target_executable_filename;

    void SetUp() override
    {
        m_run_folder_path = PathUtils::Combine(PathUtils::GetTempPath(), "test" + m_test_config.GetSlugId() + m_run_id_generator.GetRunId());
        m_run_folder_path_manager.SetRunFolderPath(m_run_folder_path);
        m_run_folder_path_manager.SetWorkingFolderPath(PathUtils::GetTempPath());

        m_target_executable_filename = m_run_folder_path_manager.MoveFileToRunFolder(TestHelper::FindExecutableFileName(m_test_config.GetExecutableName()));

        TestEnvironmentHelper::Setup(m_app_info);
        TestEnvironmentHelper::Setup(m_run_id_generator);
        TestEnvironmentHelper::Setup(m_run_folder_path_manager);

        m_run_folder_path_manager.Setup();
    }

    void TearDown() override
    {
        m_run_folder_path_manager.Cleanup();
    }
};

TEST_F(TEST_GROUP, Execute_with_help_request_produces_command_list)
{
    const auto expectedResultsFileName = m_test_config.GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "-?"));

    TestHelper::WriteMajorSeparator(100);
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "--help"));
}

TEST_F(TEST_GROUP, Execute_command_cancel_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = m_test_config.GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "cancel|-?"));
}

TEST_F(TEST_GROUP, Execute_command_elapsed_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = m_test_config.GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "elapsed|-?"));
}

TEST_F(TEST_GROUP, Execute_command_list_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = m_test_config.GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "list|-?"));
}

TEST_F(TEST_GROUP, Execute_command_pause_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = m_test_config.GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "pause|-?"));
}

TEST_F(TEST_GROUP, Execute_command_resume_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = m_test_config.GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "resume|-?"));
}

TEST_F(TEST_GROUP, Execute_command_start_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = m_test_config.GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "start|-?"));
}

TEST_F(TEST_GROUP, Execute_command_stop_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = m_test_config.GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "stop|-?"));
}
