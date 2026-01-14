#include "pch.h"

#define VER_PRODUCTNAME_STR         "BannerText.Tests"
#define VER_FILE_DESCRIPTION_STR    "BannerText Google Tests"

#include "../../../Common/AppInfo.h"
#include "../../../libs/DNX.Utils/DateUtils.h"
#include "../../../libs/DNX.Utils/DirectoryUtils.h"
#include "../../DNX.Tests.Common/TestHelper.h"
#include "../../DNX.Tests.Common/TestRunRelocatingController.h"
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
    TestRunRelocatingController* m_test_controller = nullptr;
    string m_target_executable_filename;

    void SetUp() override
    {
        m_test_controller = new TestRunRelocatingController(::testing::UnitTest::GetInstance(), static_cast<AppDetails>(m_app_info), "BannerText.exe", "bt");
        m_test_controller->SetUp();

        m_target_executable_filename = m_test_controller->GetRelocatedExecutableFilePath();
    }

    void TearDown() override
    {
        m_test_controller->TearDown();
    }
};

TEST_F(TEST_GROUP, Execute_with_help_request_produces_arguments_list)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "-?"));

    TestHelper::WriteMajorSeparator(100);
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "--help"));
}

TEST_F(TEST_GROUP, Execute_with_text_only_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName(); // string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".txt";

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "bob"));
}

TEST_F(TEST_GROUP, Execute_with_text_and_min_length_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName()    ;

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "bob|-minl|80"));
}

TEST_F(TEST_GROUP, Execute_with_multiple_text_lines_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "a|bb|ccc|dddd|eeeee"));
}

TEST_F(TEST_GROUP, Execute_with_multiple_text_lines_aligned_center_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filename, "a|bb|ccc|dddd|eeeee|-ta|Center"));
}
