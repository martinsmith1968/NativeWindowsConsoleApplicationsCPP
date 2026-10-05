#include "pch.h"

#define VER_PRODUCTNAME_STR         "BannerText.Tests"
#define VER_FILE_DESCRIPTION_STR    "BannerText Google Tests"

#include "../../../Common/AppInfo.h"
#include "../../../apps/Stopwatch/AppCommands.h"
#include "../../../libs/DNX.Utils/DirectoryUtils.h"
#include "../../../libs/DNX.Utils/FileUtils.h"
#include "../../DNX.Tests.Common/BlockTimer.h"
#include "../../DNX.Tests.Common/TestHelper.h"
#include "../../DNX.Tests.Common/TestRunRelocatingController.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable StringLiteralTypo
// ReSharper disable CppUseInternalLinkage

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

#define TEST_GROUP PauseN

//------------------------------------------------------------------------------

class TestData
{
public:
    static string GetText_CustomWithTimeoutValue()
    {
        return "Pausing for {timeout} seconds";
    }
};

//------------------------------------------------------------------------------

class TEST_GROUP : public testing::Test
{
protected:
    AppInfo m_app_info;
    TestRunController* m_test_controller = nullptr;
    string m_target_executable_filepath;

    void SetUp() override
    {
        m_test_controller = new TestRunRelocatingController(::testing::UnitTest::GetInstance(), static_cast<AppDetails>(m_app_info), "PauseN.exe", "p");
        m_test_controller->SetUp();

        m_target_executable_filepath = m_test_controller->GetExecutableFilePath();

        cout << "Current Directory: " << DirectoryUtils::GetCurrentDirectory() << endl;
    }

    void TearDown() override
    {
        m_test_controller->TearDown();
    }
};

TEST_F(TEST_GROUP, Execute_with_help_request_produces_arguments_list)
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

TEST_F(TEST_GROUP, Execute_for_1_second_default_message)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    auto timer = BlockTimer();
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "-t|1"));
    EXPECT_GE(timer.elapsed<BlockTimer::SECONDS>(), 1);
}

TEST_F(TEST_GROUP, Execute_for_5_seconds_default_message)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    auto timer = BlockTimer();
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "-t|5"));
    EXPECT_GE(timer.elapsed<BlockTimer::SECONDS>(), 3);
}

TEST_F(TEST_GROUP, Execute_for_5_seconds_custom_message)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    auto timer = BlockTimer();
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, TestData::GetText_CustomWithTimeoutValue() + "|-t|5"));
    EXPECT_GE(timer.elapsed<BlockTimer::SECONDS>(), 5);
}
