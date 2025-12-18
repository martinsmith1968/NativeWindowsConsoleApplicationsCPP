#include "pch.h"

#include "../DNX.Tests.Common/TestHelper.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/PathUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable StringLiteralTypo

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

#define TEST_GROUP BannerText

//------------------------------------------------------------------------------

class TestConfig
{
public:
    static string GetExecutableName()
    {
        return "BannerText.exe";
    }

    static string GetExpectedOutputFileName()
    {
        return PathUtils::Combine("ExpectedOutput", string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".txt");
    }
};

//------------------------------------------------------------------------------

TEST(TEST_GROUP, Execute_with_text_only_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName(); // string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".txt";

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), "bob"));
}

TEST(TEST_GROUP, Execute_with_text_and_min_length_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName()    ;

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), "bob|-minl|80"));
}

TEST(TEST_GROUP, Execute_with_multiple_text_lines_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), "a|bb|ccc|dddd|eeeee"));
}

TEST(TEST_GROUP, Execute_with_multiple_text_lines_aligned_center_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), "a|bb|ccc|dddd|eeeee|-ta|Center"));
}
