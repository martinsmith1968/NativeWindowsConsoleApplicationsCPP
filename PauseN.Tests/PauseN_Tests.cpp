#include "pch.h"

#include "../DNX.Tests.Common/BlockTimer.hpp"
#include "../DNX.Tests.Common/TestHelper.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/PathUtils.h"
#include "../Stopwatch/AppCommands.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable StringLiteralTypo

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

#define TEST_GROUP PauseN

//------------------------------------------------------------------------------

class TestConfig
{
public:
    static string GetExecutableName()
    {
        return "PauseN.exe";
    }

    static string GetExpectedOutputFileName()
    {
        return PathUtils::Combine("ExpectedOutput", string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".txt");
    }
};

class TestData
{
public:
    static string GetText_CustomWithTimeoutValue()
    {
        return "Press a key, or wait {timeout} seconds...";
    }
};

TEST(TEST_GROUP, Execute_no_parameters_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    auto timer = BlockTimer();
    EXPECT_EQ(TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), "|-t|3"), TestHelper::GetExpectedOutput(expectedResultsFileName));
    EXPECT_GE(timer.elapsed<BlockTimer::SECONDS>(), 3);
}

TEST(TEST_GROUP, Execute_custom_text_showing_timeout_value_produces_expected_output)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();

    auto timer = BlockTimer();
    EXPECT_EQ(TestHelper::ExecuteAndCaptureOutput(TestConfig::GetExecutableName(), TestData::GetText_CustomWithTimeoutValue() + "|-t|5"), TestHelper::GetExpectedOutput(expectedResultsFileName));
    EXPECT_GE(timer.elapsed<BlockTimer::SECONDS>(), 5);
}
