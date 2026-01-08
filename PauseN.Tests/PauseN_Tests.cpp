#include "pch.h"

#define VER_PRODUCTNAME_STR         "BannerText.Tests"
#define VER_FILE_DESCRIPTION_STR    "BannerText Google Tests"

#include "../Common/AppInfo.h"
#include "../DNX.Tests.Common/BlockTimer.h"
#include "../DNX.Tests.Common/TestHelper.h"
#include "../DNX.Tests.Common/TestRunController.h"
#include "../DNX.Utils/FileUtils.h"
#include "../Stopwatch/AppCommands.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable StringLiteralTypo

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
        return "Press a key, or wait {timeout} seconds...";
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
        m_test_controller = new TestRunController(::testing::UnitTest::GetInstance(), static_cast<AppDetails>(m_app_info), "PauseN.exe", "p");
        m_test_controller->SetUp();

        m_target_executable_filepath = m_test_controller->GetExecutableFilePath();
    }

    void TearDown() override
    {
        m_test_controller->TearDown();
    }
};

TEST_F(TEST_GROUP, Execute_no_parameters_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    auto timer = BlockTimer();
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "|-t|3"));
    EXPECT_GE(timer.elapsed<BlockTimer::SECONDS>(), 3);
}

TEST_F(TEST_GROUP, Execute_custom_text_showing_timeout_value_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    auto timer = BlockTimer();
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, TestData::GetText_CustomWithTimeoutValue() + "|-t|5"));
    EXPECT_GE(timer.elapsed<BlockTimer::SECONDS>(), 5);
}
