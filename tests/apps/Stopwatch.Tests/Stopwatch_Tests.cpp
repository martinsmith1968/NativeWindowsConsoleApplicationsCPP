#include "pch.h"

#define VER_PRODUCTNAME_STR         "Stopwatch.Tests"
#define VER_FILE_DESCRIPTION_STR    "Stopwatch Google Tests"

#include "../../../Common/AppInfo.h"
#include "../../../libs/DNX.Utils/DateUtils.h"
#include "../../../libs/DNX.Utils/DirectoryUtils.h"
#include "../../DNX.Tests.Common/TestHelper.h"
#include "../../DNX.Tests.Common/TestRunRelocatingController.h"
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
    TestRunRelocatingController* m_test_controller = nullptr;
    string m_target_executable_filepath;

    void SetUp() override
    {
        m_test_controller = new TestRunRelocatingController(::testing::UnitTest::GetInstance(), static_cast<AppDetails>(m_app_info), "Stopwatch.exe", "sw");
        m_test_controller->SetUp();

        m_target_executable_filepath = m_test_controller->GetRelocatedExecutableFilePath();
    }

    void TearDown() override
    {
        m_test_controller->TearDown();
    }
};

TEST_F(TEST_GROUP, Execute_with_help_request_produces_command_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "-?"));

    TestHelper::WriteMajorSeparator(100);
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "--help"));
}

TEST_F(TEST_GROUP, Execute_command_cancel_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "cancel|-?"));
}

TEST_F(TEST_GROUP, Execute_command_elapsed_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "elapsed|-?"));
}

TEST_F(TEST_GROUP, Execute_command_list_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "list|-?"));
}

TEST_F(TEST_GROUP, Execute_command_pause_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "pause|-?"));
}

TEST_F(TEST_GROUP, Execute_command_resume_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "resume|-?"));
}

TEST_F(TEST_GROUP, Execute_command_start_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "start|-?"));
}

TEST_F(TEST_GROUP, Execute_command_stop_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "stop|-?"));
}
