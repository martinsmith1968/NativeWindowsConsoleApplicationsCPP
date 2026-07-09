#include "pch.h"

#define VER_PRODUCTNAME_STR         "ShowDateTime.Tests"
#define VER_FILE_DESCRIPTION_STR    "ShowDateTime Google Tests"

#include "../../../Common/AppInfo.h"
#include "../../../libs/DNX.Utils/StringUtils.h"
#include "../../DNX.Tests.Common/TestHelper.h"
#include "../../DNX.Tests.Common/TestRunController.h"
#include <regex>

// ReSharper disable CppInconsistentNaming
// ReSharper disable StringLiteralTypo

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

#define TEST_GROUP ShowDateTime

//------------------------------------------------------------------------------

class TestData
{
public:
    static const regex& GetDefaultFormatPattern()
    {
        static const regex pattern(R"(^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{3}$)");
        return pattern;
    }

    static const regex& GetDateOnlyFormatPattern()
    {
        static const regex pattern(R"(^\d{4}/\d{2}/\d{2}$)");
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
        m_test_controller = new TestRunController(::testing::UnitTest::GetInstance(), static_cast<AppDetails>(m_app_info), "ShowDateTime.exe", "sdt");
        m_test_controller->SetUp();

        m_target_executable_filepath = m_test_controller->GetExecutableFilePath();
    }

    void TearDown() override
    {
        m_test_controller->TearDown();
    }

    static string GetOutputLine(const string& output)
    {
        auto start = output.find_first_not_of(" \t\r\n");
        auto end = output.find_last_not_of(" \t\r\n");

        if (start == string::npos)
            return "";

        return output.substr(start, end - start + 1);
    }
};

TEST_F(TEST_GROUP, Execute_no_parameters_produces_default_formatted_local_datetime)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "");

    const auto line = GetOutputLine(output);

    EXPECT_TRUE(regex_match(line, TestData::GetDefaultFormatPattern()));
}

TEST_F(TEST_GROUP, Execute_with_type_UTC_produces_default_formatted_utc_datetime)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "|-t|UTC");

    const auto line = GetOutputLine(output);

    EXPECT_TRUE(regex_match(line, TestData::GetDefaultFormatPattern()));
}

TEST_F(TEST_GROUP, Execute_with_type_Local_produces_default_formatted_local_datetime)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "|-t|Local");

    const auto line = GetOutputLine(output);

    EXPECT_TRUE(regex_match(line, TestData::GetDefaultFormatPattern()));
}

TEST_F(TEST_GROUP, Execute_with_custom_format_produces_date_only_output)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, R"(|-f|%Y/%m/%d)");

    const auto line = GetOutputLine(output);

    EXPECT_TRUE(regex_match(line, TestData::GetDateOnlyFormatPattern()));
}

TEST_F(TEST_GROUP, Execute_with_custom_format_replaces_millisecond_tokens)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, R"(|-f|{fff}-{f})");

    const auto line = GetOutputLine(output);

    static const regex pattern(R"(^\d{3}-\d{1,3}$)");
    EXPECT_TRUE(regex_match(line, pattern));
}

TEST_F(TEST_GROUP, Execute_with_fixed_datetime_produces_that_date_at_midnight)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "|-dt|2024-03-17");

    const auto line = GetOutputLine(output);

    EXPECT_EQ("2024-03-17 00:00:00.000", line);
}

TEST_F(TEST_GROUP, Execute_with_fixed_datetime_and_custom_format_produces_expected_date)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, R"(|-dt|2024-03-17|-f|%Y-%m-%d)");

    const auto line = GetOutputLine(output);

    EXPECT_EQ("2024-03-17", line);
}

TEST_F(TEST_GROUP, Execute_with_fixed_datetime_and_type_UTC_produces_same_fixed_datetime)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "|-dt|2024-03-17|-t|UTC");

    const auto line = GetOutputLine(output);

    EXPECT_EQ("2024-03-17 00:00:00.000", line);
}

TEST_F(TEST_GROUP, Execute_with_invalid_fixed_datetime_produces_error_and_nonzero_exitcode)
{
    const auto output = TestHelper::ExecuteAndCaptureOutput(m_target_executable_filepath, "|-dt|not-a-date");

    EXPECT_NE(output.find("ERROR: Option: datetime value is invalid (not-a-date)"), string::npos);
}
