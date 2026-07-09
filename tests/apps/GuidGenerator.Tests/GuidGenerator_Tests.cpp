#include "pch.h"

#define VER_PRODUCTNAME_STR         "GuidGenerator.Tests"
#define VER_FILE_DESCRIPTION_STR    "GuidGenerator Google Tests"

#include "../../../Common/AppInfo.h"
#include "../../../libs/DNX.Utils/StringUtils.h"
#include "../../DNX.Tests.Common/TestHelper.h"
#include "../../DNX.Tests.Common/TestRunController.h"
#include <regex>
#include <vector>

// ReSharper disable CppInconsistentNaming
// ReSharper disable StringLiteralTypo

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

#define TEST_GROUP GuidGenerator

//------------------------------------------------------------------------------

class TestData
{
public:
    static const regex& GetDefaultFormatPattern()
    {
        static const regex pattern(R"(^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$)");
        return pattern;
    }

    static const regex& GetDigitsFormatPattern()
    {
        static const regex pattern(R"(^[0-9a-f]{32}$)");
        return pattern;
    }

    static const regex& GetBracedFormatPattern()
    {
        static const regex pattern(R"(^\{[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}\}$)");
        return pattern;
    }

    static const regex& GetParenthesizedFormatPattern()
    {
        static const regex pattern(R"(^\([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}\)$)");
        return pattern;
    }

    static const regex& GetUpperCaseDefaultFormatPattern()
    {
        static const regex pattern(R"(^[0-9A-F]{8}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{12}$)");
        return pattern;
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
        m_test_controller = new TestRunController(::testing::UnitTest::GetInstance(), static_cast<AppDetails>(m_app_info), "GuidGenerator.exe", "gg");
        m_test_controller->SetUp();

        m_target_executable_filepath = m_test_controller->GetExecutableFilePath();
    }

    void TearDown() override
    {
        m_test_controller->TearDown();
    }

    static vector<string> GetOutputLines(const string& output)
    {
        const auto split_lines = StringUtils::SplitText(StringUtils::Trim(output), '\n');

        vector<string> lines;
        for (const auto& line : split_lines)
        {
            lines.push_back(StringUtils::Trim(line, "\r"));
        }
        return lines;
    }
};

TEST_F(TEST_GROUP, Execute_no_parameters_produces_single_guid_in_default_format)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "");

    const auto lines = GetOutputLines(output);

    ASSERT_EQ(1, lines.size());
    EXPECT_TRUE(regex_match(lines[0], TestData::GetDefaultFormatPattern()));
}

TEST_F(TEST_GROUP, Execute_with_count_produces_multiple_guids)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "|-c|5");

    const auto lines = GetOutputLines(output);

    ASSERT_EQ(5, lines.size());
    for (const auto& line : lines)
    {
        EXPECT_TRUE(regex_match(line, TestData::GetDefaultFormatPattern()));
    }
}

TEST_F(TEST_GROUP, Execute_with_format_n_produces_32_digit_guid)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "|-f|n");

    const auto lines = GetOutputLines(output);

    ASSERT_EQ(1, lines.size());
    EXPECT_TRUE(regex_match(lines[0], TestData::GetDigitsFormatPattern()));
}

TEST_F(TEST_GROUP, Execute_with_format_d_produces_dashed_guid)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "|-f|d");

    const auto lines = GetOutputLines(output);

    ASSERT_EQ(1, lines.size());
    EXPECT_TRUE(regex_match(lines[0], TestData::GetDefaultFormatPattern()));
}

TEST_F(TEST_GROUP, Execute_with_format_b_produces_braced_guid)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "|-f|b");

    const auto lines = GetOutputLines(output);

    ASSERT_EQ(1, lines.size());
    EXPECT_TRUE(regex_match(lines[0], TestData::GetBracedFormatPattern()));
}

TEST_F(TEST_GROUP, Execute_with_format_p_produces_parenthesized_guid)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "|-f|p");

    const auto lines = GetOutputLines(output);

    ASSERT_EQ(1, lines.size());
    EXPECT_TRUE(regex_match(lines[0], TestData::GetParenthesizedFormatPattern()));
}

TEST_F(TEST_GROUP, Execute_with_format_D_produces_uppercase_dashed_guid)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "|-f|D");

    const auto lines = GetOutputLines(output);

    ASSERT_EQ(1, lines.size());
    EXPECT_TRUE(regex_match(lines[0], TestData::GetUpperCaseDefaultFormatPattern()));
}

TEST_F(TEST_GROUP, Execute_with_output_format_produces_custom_template)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, R"(|-c|3|-of|{seq}:{id}:{guid})");

    const auto lines = GetOutputLines(output);

    ASSERT_EQ(3, lines.size());

    const regex pattern(R"(^([1-3]):(\d):([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12})$)");
    for (auto index = 0; index < static_cast<int>(lines.size()); ++index)
    {
        smatch match;
        ASSERT_TRUE(regex_match(lines[index], match, pattern));
        EXPECT_EQ(to_string(index + 1), match[1].str());
        EXPECT_EQ(to_string(index + 1), match[2].str());
    }
}
