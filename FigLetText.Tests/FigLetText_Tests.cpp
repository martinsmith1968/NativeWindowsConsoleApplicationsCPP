#include "pch.h"

#include "TestHelper.h"
#include "../DNX.Utils/EnvironmentUtils.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/PathUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable StringLiteralTypo

using namespace std;
using namespace DNX::Utils;

#define TEST_GROUP FigLetText

const string HelloWorld_Text = "HelloWorld!";

//------------------------------------------------------------------------------

string GetExpectedOutputFileName()
{
    return PathUtils::Combine("ExpectedOutput", string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".txt");
}

TEST(TEST_GROUP, Execute_HelloWorld_default_font_produces_expected_output)
{
    const auto expectedResultsFileName = GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteApp(HelloWorld_Text), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_banner_produces_expected_output)
{
    const auto expectedResultsFileName = GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteApp(HelloWorld_Text + "|-n|banner"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_big_produces_expected_output)
{
    const auto expectedResultsFileName = GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteApp(HelloWorld_Text + "|-n|big"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_doom_produces_expected_output)
{
    const auto expectedResultsFileName = GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteApp(HelloWorld_Text + "|-n|doom"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_larry3d_produces_expected_output)
{
    const auto expectedResultsFileName = GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteApp(HelloWorld_Text + "|-n|larry3d"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_mini_produces_expected_output)
{
    const auto expectedResultsFileName = GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteApp(HelloWorld_Text + "|-n|mini"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_script_produces_expected_output)
{
    const auto expectedResultsFileName = GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteApp(HelloWorld_Text + "|-n|script"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_small_produces_expected_output)
{
    const auto expectedResultsFileName = GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteApp(HelloWorld_Text + "|-n|small"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_standard_produces_expected_output)
{
    const auto expectedResultsFileName = GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteApp(HelloWorld_Text + "|-n|standard"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_HelloWorld_font_straight_produces_expected_output)
{
    const auto expectedResultsFileName = GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteApp(HelloWorld_Text + "|-n|straight"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}
