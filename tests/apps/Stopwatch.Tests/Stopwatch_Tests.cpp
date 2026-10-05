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
#include <iostream>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable StringLiteralTypo
// ReSharper disable CppUseInternalLinkage

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

        cout << "Current Directory: " << DirectoryUtils::GetCurrentDirectory() << endl;
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
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "/?"));
}

TEST_F(TEST_GROUP, Execute_with_full_help_request_produces_arguments_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "--help"));
}

TEST_F(TEST_GROUP, Execute_with_help_request_for_Cancel_produces_arguments_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "cancel|-?"));
}

TEST_F(TEST_GROUP, Execute_with_help_request_for_Elapsed_produces_arguments_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "elapsed|-?"));
}

TEST_F(TEST_GROUP, Execute_with_help_request_for_List_produces_arguments_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "list|-?"));
}

TEST_F(TEST_GROUP, Execute_with_help_request_for_Pause_produces_arguments_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "pause|-?"));
}

TEST_F(TEST_GROUP, Execute_with_help_request_for_Purge_produces_arguments_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "purge|-?"));
}

TEST_F(TEST_GROUP, Execute_with_help_request_for_Resume_produces_arguments_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "resume|-?"));
}

TEST_F(TEST_GROUP, Execute_with_help_request_for_Start_produces_arguments_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "start|-?"));
}

TEST_F(TEST_GROUP, Execute_with_help_request_for_Stop_produces_arguments_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "stop|-?"));
}
