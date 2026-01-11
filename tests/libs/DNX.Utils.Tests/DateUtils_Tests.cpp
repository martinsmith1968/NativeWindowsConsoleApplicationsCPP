#include "pch.h"

#include "../DNX.Utils/DateUtils.h"
#include "../DNX.Utils/StringUtils.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppTooWideScope

#define TEST_GROUP DateUtils

TEST(TEST_GROUP, GetNow_returns_a_value)
{
    // Act
    const auto result = DateUtils::GetNow();
    cout << "Result: " << result << endl;

    // Assert
    EXPECT_GT(result, 0);
}

TEST(TEST_GROUP, ToCalendarDateTime_and_ToEpochDateTime_work_interchangeably)
{
    const auto now = DateUtils::GetNow();

    // Act
    auto calendar_time = DateUtils::ToCalendarDateTime(now);
    const auto epoch_time = DateUtils::ToEpochDateTime(&calendar_time);

    // Assert
    EXPECT_EQ(epoch_time, now);
}

TEST(TEST_GROUP, FormatDate_with_only_modifiers_in_the_format_returns_sucessfully)
{
    const auto tmt = time(nullptr);
    const auto tm = DateUtils::ToCalendarDateTime(tmt);

    const string format = "%Y-%m-%d %H:%M:%S";

    string expected_result = format;
    expected_result = StringUtils::Replace(expected_result, "%Y", to_string(tm.tm_year));
    expected_result = StringUtils::Replace(expected_result, "%m", StringUtils::LPad(to_string(tm.tm_mon), 2, '0'));
    expected_result = StringUtils::Replace(expected_result, "%d", StringUtils::LPad(to_string(tm.tm_mday), 2, '0'));
    expected_result = StringUtils::Replace(expected_result, "%H", StringUtils::LPad(to_string(tm.tm_hour), 2, '0'));
    expected_result = StringUtils::Replace(expected_result, "%M", StringUtils::LPad(to_string(tm.tm_min), 2, '0'));
    expected_result = StringUtils::Replace(expected_result, "%S", StringUtils::LPad(to_string(tm.tm_sec), 2, '0'));

    // Act
    const auto result = DateUtils::FormatDate(&tm, format);
    cout << "Result: " << result << endl;

    // Assert
    EXPECT_GT(result, expected_result);
}

TEST(TEST_GROUP, FormatDate_with_text_and_modifiers_in_the_format_returns_sucessfully)
{
    const auto tmt = time(nullptr);
    const auto tm = DateUtils::ToCalendarDateTime(tmt);

    const string format = "Today's Date is %Y-%m-%d, and the current time is %H:%M:%S";

    string expected_result = format;
    expected_result = StringUtils::Replace(expected_result, "%Y", to_string(tm.tm_year));
    expected_result = StringUtils::Replace(expected_result, "%m", StringUtils::LPad(to_string(tm.tm_mon), 2, '0'));
    expected_result = StringUtils::Replace(expected_result, "%d", StringUtils::LPad(to_string(tm.tm_mday), 2, '0'));
    expected_result = StringUtils::Replace(expected_result, "%H", StringUtils::LPad(to_string(tm.tm_hour), 2, '0'));
    expected_result = StringUtils::Replace(expected_result, "%M", StringUtils::LPad(to_string(tm.tm_min), 2, '0'));
    expected_result = StringUtils::Replace(expected_result, "%S", StringUtils::LPad(to_string(tm.tm_sec), 2, '0'));

    // Act
    const auto result = DateUtils::FormatDate(&tm, format);
    cout << "Result: " << result << endl;

    // Assert
    EXPECT_GT(result, expected_result);
}
