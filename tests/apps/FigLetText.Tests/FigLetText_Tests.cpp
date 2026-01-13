#include "pch.h"
#include "../../../libs/DNX.Utils/FileUtils.h"
#include "../../../libs/DNX.Utils/PathUtils.h"
#include "../../DNX.Tests.Common/TestHelper.h"

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

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld()));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_banner_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|banner"));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_big_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|big"));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_doom_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|doom"));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_larry3d_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|larry3d"));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_mini_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|mini"));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_script_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|script"));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_small_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|small"));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_standard_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|standard"));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_straight_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_HelloWorld() + "|-n|straight"));
}
