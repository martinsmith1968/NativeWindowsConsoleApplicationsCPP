#include "pch.h"

#include "TestHelper.h"
#include "../DNX.Utils/EnvironmentUtils.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/PathUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable StringLiteralTypo

using namespace std;
using namespace DNX::Utils;

#define TEST_GROUP BannerText

//------------------------------------------------------------------------------

string GetExpectedOutputFileName()
{
    return PathUtils::Combine("ExpectedOutput", string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".txt");
}

TEST(TEST_GROUP, Execute_with_text_only_produces_expected_output)
{
    const auto expectedResultsFileName = GetExpectedOutputFileName(); // string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".txt";

    EXPECT_EQ(TestHelper::ExecuteApp("bob"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_with_text_and_min_length_produces_expected_output)
{
    const auto expectedResultsFileName = GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteApp("bob|-minl 80"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_with_multiple_text_lines_produces_expected_output)
{
    const auto expectedResultsFileName = GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteApp("a|bb|ccc|dddd|eeeee"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_with_multiple_text_lines_aligned_center_produces_expected_output)
{
    const auto expectedResultsFileName = GetExpectedOutputFileName();

    EXPECT_EQ(TestHelper::ExecuteApp("a|bb|ccc|dddd|eeeee|-ta Center"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}
