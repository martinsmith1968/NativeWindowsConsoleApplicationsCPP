#include "pch.h"

#define VER_PRODUCTNAME_STR         "BannerText.Tests"
#define VER_FILE_DESCRIPTION_STR    "BannerText Google Tests"

#include "../Common/AppInfo.h"
#include "../DNX.Tests.Common/RunFolderPathManager.h"
#include "../DNX.Tests.Common/RunIdGenerator.h"
#include "../DNX.Tests.Common/TestConfig.h"
#include "../DNX.Tests.Common/TestEnvironmentHelper.h"
#include "../DNX.Tests.Common/TestHelper.h"
#include "../DNX.Utils/DateUtils.h"
#include "../DNX.Utils/DirectoryUtils.h"
#include "../DNX.Utils/PathUtils.h"
#include <filesystem>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable StringLiteralTypo

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

#define TEST_GROUP BannerText

//------------------------------------------------------------------------------

class TEST_GROUP : public testing::Test
{
protected:
    AppInfo m_app_info;
    RunIdGenerator m_run_id_generator;
    RunFolderPathManager m_run_folder_path_manager;
    TestConfig m_test_config = TestConfig(::testing::UnitTest::GetInstance(), "BannerText.exe", "bt");

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

TEST_F(TEST_GROUP, Execute_with_help_request_produces_arguments_list)
{
    const auto expectedResultsFileName = m_test_config.GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "-?"));

    TestHelper::WriteMajorSeparator(100);
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "--help"));
}

TEST_F(TEST_GROUP, Execute_with_text_only_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_config.GetExpectedOutputFileName(); // string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".txt";

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "bob"));
}

TEST_F(TEST_GROUP, Execute_with_text_and_min_length_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_config.GetExpectedOutputFileName()    ;

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "bob|-minl|80"));
}

TEST_F(TEST_GROUP, Execute_with_multiple_text_lines_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_config.GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "a|bb|ccc|dddd|eeeee"));
}

TEST_F(TEST_GROUP, Execute_with_multiple_text_lines_aligned_center_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_config.GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "a|bb|ccc|dddd|eeeee|-ta|Center"));
}
