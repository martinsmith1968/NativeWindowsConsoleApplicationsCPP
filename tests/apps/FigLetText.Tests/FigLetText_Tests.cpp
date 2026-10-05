#include "pch.h"

#define VER_PRODUCTNAME_STR         "FigLetText.Tests"
#define VER_FILE_DESCRIPTION_STR    "FigLetText Google Tests"

#include "../../../Common/AppInfo.h"
#include "../../../libs/DNX.Utils/DateUtils.h"
#include "../../../libs/DNX.Utils/DirectoryUtils.h"
#include "../../DNX.Tests.Common/TestHelper.h"
#include "../../DNX.Tests.Common/TestRunRelocatingController.h"
#include <filesystem>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable StringLiteralTypo
// ReSharper disable CppUseInternalLinkage

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

#define TEST_GROUP FigLetText

//------------------------------------------------------------------------------

class TestData
{
public:
    static string GetText_HelloWorld()
    {
        return "HelloWorld!";
    }
};

//------------------------------------------------------------------------------

class TEST_GROUP : public testing::Test
{
protected:
    AppInfo m_app_info;
    TestRunRelocatingController* m_test_controller = nullptr;
    string m_target_executable_filepath;

    void SetUp() override
    {
        m_test_controller = new TestRunRelocatingController(::testing::UnitTest::GetInstance(), static_cast<AppDetails>(m_app_info), "FigLetText.exe", "fl");
        m_test_controller->SetUp();

        m_target_executable_filepath = m_test_controller->GetRelocatedExecutableFilePath();

        cout << "Current Directory: " << DirectoryUtils::GetCurrentDirectory() << endl;
    }

    void TearDown() override
    {
        m_test_controller->TearDown();
    }
};

TEST_F(TEST_GROUP, Execute_HelloWorld_default_font_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, TestData::GetText_HelloWorld()));
}

TEST_F(TEST_GROUP, Execute_HelloWorld_font_banner_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, TestData::GetText_HelloWorld() + "|-n|banner"));
}

TEST_F(TEST_GROUP, Execute_HelloWorld_font_big_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, TestData::GetText_HelloWorld() + "|-n|big"));
}

TEST_F(TEST_GROUP, Execute_HelloWorld_font_doom_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, TestData::GetText_HelloWorld() + "|-n|doom"));
}

TEST_F(TEST_GROUP, Execute_HelloWorld_font_larry3d_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, TestData::GetText_HelloWorld() + "|-n|larry3d"));
}

TEST_F(TEST_GROUP, Execute_HelloWorld_font_mini_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, TestData::GetText_HelloWorld() + "|-n|mini"));
}

TEST_F(TEST_GROUP, Execute_HelloWorld_font_script_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, TestData::GetText_HelloWorld() + "|-n|script"));
}

TEST_F(TEST_GROUP, Execute_HelloWorld_font_small_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, TestData::GetText_HelloWorld() + "|-n|small"));
}

TEST_F(TEST_GROUP, Execute_HelloWorld_font_standard_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, TestData::GetText_HelloWorld() + "|-n|standard"));
}

TEST_F(TEST_GROUP, Execute_HelloWorld_font_straight_produces_expected_output)
{
    const auto expectedResultsFileName = m_test_controller->GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, TestData::GetText_HelloWorld() + "|-n|straight"));
}
