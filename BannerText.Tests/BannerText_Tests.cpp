#include "pch.h"

#include "TestHelper.h"
#include "../DNX.Utils/EnvironmentUtils.h"
#include "../DNX.Utils/FileUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceUnnecessaryCopyInitialization
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable StringLiteralTypo
// ReSharper disable CppClangTidyMiscUseAnonymousNamespace

using namespace std;
using namespace DNX::Utils;

#define TEST_GROUP BannerText

//------------------------------------------------------------------------------

TEST(TEST_GROUP, Execute_with_text_only_produces_expected_output)
{
    const auto expectedResultsFileName = string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".txt";

    EXPECT_EQ(TestHelper::ExecuteApp("bob"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST(TEST_GROUP, Execute_with_text_and_min_length_produces_expected_output)
{
    const auto expectedResultsFileName = string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".txt";

    EXPECT_EQ(TestHelper::ExecuteApp("bob|-minl 80"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}
