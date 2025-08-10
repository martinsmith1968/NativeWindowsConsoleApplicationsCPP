#include "pch.h"

#include "../DNX.Tests.Common/TestHelper.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/PathUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable StringLiteralTypo

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

#define TEST_GROUP FigLetText

//------------------------------------------------------------------------------

class TestConfig
{
public:
    static string GetExecutableName()
    {
        return "FigLetText.exe";
    }

    static string GetExpectedOutputFileName()
    {
        return PathUtils::Combine("ExpectedOutput", string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".txt");
    }
};

class TestData
{
public:
    static string GetText_HelloWorld()
    {
        return "HelloWorld!";
    }
};

TEST(TEST_GROUP, Execute_HelloWorld_default_font_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld()), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_banner_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|banner"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_big_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|big"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_doom_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|doom"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_larry3d_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|larry3d"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_mini_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|mini"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_script_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|script"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_small_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|small"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_standard_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|standard"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_straight_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|straight"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}
