#include "pch.h"

#include <thread>

#include "../../../libs/DNX.Utils/DateTime.h"
#include "../../../libs/DNX.Utils/StringUtils.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppTooWideScope
// ReSharper disable CppClangTidyMiscUseAnonymousNamespace
// ReSharper disable CppClangTidyClangDiagnosticPadded
// ReSharper disable CppClangTidyClangDiagnosticVariadicMacroArgumentsOmitted
// ReSharper disable CppDeclaratorNeverUsed

#define TEST_GROUP DateTime

#define SHOW_DATETIME(var) ShowDateTime(var, #var)
#define SHOW_DATETIME_NAMED(var, name) ShowDateTime(var, name)

//--------------------------------------------------------------------------------
// See Also:
// - https://www.sandordargo.com/blog/2019/04/24/parameterized-testing-with-gtest
// - https://blog.andreiavram.ro/gtest-parameterized-tests-json/


//--------------------------------------------------------------------------------
// Helper Methods
static void ShowDateTime(const DateTime& dateTime, string name = "")
{
    name = (name.empty())
        ? "DateTime"
        : StringUtils::LPad(name, 8);

    cout << StringUtils::Repeat("-", 35) << endl;
    cout << name << ": " << dateTime.ToString() << endl;
    cout << "    Year: " << dateTime.GetYear() << endl;
    cout << " Quarter: " << dateTime.GetQuarter() << endl;
    cout << "   Month: " << dateTime.GetMonth() << endl;
    cout << "     Day: " << dateTime.GetDay() << endl;
    cout << "    Hour: " << dateTime.GetHour() << endl;
    cout << "  Minute: " << dateTime.GetMinute() << endl;
    cout << "  Second: " << dateTime.GetSeconds() << endl;
    cout << "      ms: " << dateTime.GetMilliseconds() << endl;
}

static void ShowDuration(const chrono::system_clock::duration duration)
{
    const auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration - hours);
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration - hours - minutes);
    const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration - hours - minutes - seconds);
    const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration - hours - minutes - seconds - milliseconds);
    const auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration - hours - minutes - seconds - milliseconds - microseconds);

    std::cout << hours.count() << "h ";
    std::cout << minutes.count() << "m ";
    std::cout << seconds.count() << "s ";
    std::cout << milliseconds.count() << "ms ";
    std::cout << microseconds.count() << "us ";
    std::cout << nanoseconds.count() << "ns" << endl;
}

static void VerifyDateTime(const DateTime& dateTime)
{
    EXPECT_NE(dateTime.ToString(), "");
    EXPECT_GT(dateTime.GetYear(), 0);
    EXPECT_GE(dateTime.GetQuarter(), 1);
    EXPECT_LE(dateTime.GetQuarter(), 4);
    EXPECT_GE(dateTime.GetMonth(), 1);
    EXPECT_LE(dateTime.GetMonth(), 12);
    EXPECT_GE(dateTime.GetDay(), 1);
    EXPECT_LE(dateTime.GetDay(), 31);
    EXPECT_GE(dateTime.GetHour(), 0);
    EXPECT_LE(dateTime.GetHour(), 23);
    EXPECT_GE(dateTime.GetMinute(), 0);
    EXPECT_LE(dateTime.GetMinute(), 59);
    EXPECT_GE(dateTime.GetSeconds(), 0);
    EXPECT_LE(dateTime.GetSeconds(), 59);
    EXPECT_GE(dateTime.GetMilliseconds(), 0);
}

static std::chrono::system_clock::duration get_duration_since_midnight()
{
    const auto now = std::chrono::system_clock::now();

    const time_t time_t = std::chrono::system_clock::to_time_t(now);
    tm date;

    const auto error = localtime_s(&date, &time_t);
    date.tm_hour = 0;
    date.tm_min = 0;
    date.tm_sec = 0;

    const auto midnight = std::chrono::system_clock::from_time_t(std::mktime(&date));

    return now - midnight;
}


//--------------------------------------------------------------------------------
// Static Tests
TEST(TEST_GROUP, Milliseconds_test)
{
    for (auto x = 0; x < 10; ++x)
    {
        this_thread::sleep_for(std::chrono::milliseconds(10));
        const auto since_midnight = get_duration_since_midnight();
        ShowDuration(since_midnight);
    }
}

TEST(TEST_GROUP, Static_constructor_Now_returns_a_value)
{
    // Act
    const auto result = DateTime::Now();
    SHOW_DATETIME(result);

    // Assert
    VerifyDateTime(result);
}

TEST(TEST_GROUP, Static_constructor_Now_returns_differing_values)
{
    // Act
    const auto result1 = DateTime::Now();
    SHOW_DATETIME(result1);

    this_thread::sleep_for(std::chrono::seconds(2));
    const auto result2 = DateTime::Now();
    SHOW_DATETIME(result2);

    this_thread::sleep_for(std::chrono::seconds(2));
    const auto result3 = DateTime::Now();
    SHOW_DATETIME(result3);

    // Assert
    VerifyDateTime(result1);
    VerifyDateTime(result2);
    VerifyDateTime(result3);
    EXPECT_NE(result1.ToString(), result2.ToString());
    EXPECT_NE(result1.ToString(), result3.ToString());
    EXPECT_NE(result2.ToString(), result3.ToString());
    EXPECT_NE(result1.ToString(DateTime::Formats::ISO), result2.ToString(DateTime::Formats::ISO));
    EXPECT_NE(result1.ToString(DateTime::Formats::ISO), result3.ToString(DateTime::Formats::ISO));
    EXPECT_NE(result2.ToString(DateTime::Formats::ISO), result3.ToString(DateTime::Formats::ISO));
    EXPECT_NE(result1.GetSeconds(), result2.GetSeconds());
    EXPECT_NE(result1.GetSeconds(), result3.GetSeconds());
    EXPECT_NE(result2.GetSeconds(), result3.GetSeconds());
    EXPECT_NE(result1.GetMilliseconds(), result2.GetMilliseconds());
}

TEST(TEST_GROUP, Static_GetDaysInMonth_returns_appropriate_values)
{
    // Assert
    int month = 0;
    EXPECT_EQ(DateTime::GetDaysInMonth(++month), 31);
    EXPECT_EQ(DateTime::GetDaysInMonth(++month), 28);
    EXPECT_EQ(DateTime::GetDaysInMonth(++month), 31);
    EXPECT_EQ(DateTime::GetDaysInMonth(++month), 30);
    EXPECT_EQ(DateTime::GetDaysInMonth(++month), 31);
    EXPECT_EQ(DateTime::GetDaysInMonth(++month), 30);
    EXPECT_EQ(DateTime::GetDaysInMonth(++month), 31);
    EXPECT_EQ(DateTime::GetDaysInMonth(++month), 31);
    EXPECT_EQ(DateTime::GetDaysInMonth(++month), 30);
    EXPECT_EQ(DateTime::GetDaysInMonth(++month), 31);
    EXPECT_EQ(DateTime::GetDaysInMonth(++month), 30);
    EXPECT_EQ(DateTime::GetDaysInMonth(++month), 31);

    EXPECT_EQ(DateTime::GetDaysInMonth(0), DateTime::GetDaysInMonth(12));
    EXPECT_EQ(DateTime::GetDaysInMonth(13), DateTime::GetDaysInMonth(1));
}

TEST(TEST_GROUP, ToString_built_in_formats_format_as_expected)
{
    const auto test_date = DateTime(2023, 10, 28, 14, 30, 45, 123);

    // Act & Assert
    ASSERT_EQ(test_date.ToString(DateTime::Formats::Default), "Sat, Oct 28 14:30:45.1230000 2023");
    ASSERT_EQ(test_date.ToString(DateTime::Formats::Sortable), "2023-10-28 14:30:45.1230000+0000");
    ASSERT_EQ(test_date.ToString(DateTime::Formats::ISO), "2023-10-28T14:30:45.1230000+0000");
    ASSERT_EQ(test_date.ToString(DateTime::Formats::Display), "2023-10-28 14:30:45");

    ASSERT_EQ(test_date.ToString(DateTime::Formats::Date_Default), "Sat, Oct 28 2023");
    ASSERT_EQ(test_date.ToString(DateTime::Formats::Date_Sortable), "2023-10-28");
    EXPECT_EQ(test_date.ToString(DateTime::Formats::Date_ISO), "2023-10-28");
    EXPECT_EQ(test_date.ToString(DateTime::Formats::Date_Display), "2023-10-28");
}

//--------------------------------------------------------------------------------
// Test Fixtures

/// <summary>
///
/// </summary>
class DateTimeConstructorFixture
    : public ::testing::Test
{
public:
    DateTime FixtureDateTime;

    void SetUp() override = 0;
};

/// <summary>
///
/// </summary>
class DateTimeDateOnlyParseFixture
    : public DateTimeConstructorFixture
    , public ::testing::WithParamInterface<std::tuple<string, int, int, int>>
{
public:
    string InputText;
    int Year  = 0;
    int Month = 0;
    int Day   = 0;

    void SetUp() override
    {
        InputText = std::get<0>(GetParam());
        Year      = std::get<1>(GetParam());
        Month     = std::get<2>(GetParam());
        Day       = std::get<3>(GetParam());

        FixtureDateTime = DateTime::Parse(InputText);
        SHOW_DATETIME(FixtureDateTime);
    }
};

/// <summary>
///
/// </summary>
class DateTimeDateOnlyConstructorFixture
    : public DateTimeConstructorFixture
    , public ::testing::WithParamInterface<std::tuple<int, int, int>>
{
public:
    int Year  = 0;
    int Month = 0;
    int Day   = 0;

    void SetUp() override
    {
        Year  = std::get<0>(GetParam());
        Month = std::get<1>(GetParam());
        Day   = std::get<2>(GetParam());

        FixtureDateTime = DateTime(Year, Month, Day);
        SHOW_DATETIME(FixtureDateTime);
    }
};

/// <summary>
///
/// </summary>
class DateTimeDateAndTimeConstructorFixture
    : public DateTimeConstructorFixture
    , public ::testing::WithParamInterface<std::tuple<int, int, int, int, int, int, int>>
{
public:
    int Year        = 0;
    int Month       = 0;
    int Day         = 0;
    int Hour        = 0;
    int Minute      = 0;
    int Second      = 0;
    int Millisecond = 0;

    void SetUp() override
    {
        Year        = std::get<0>(GetParam());
        Month       = std::get<1>(GetParam());
        Day         = std::get<2>(GetParam());
        Hour        = std::get<3>(GetParam());
        Minute      = std::get<4>(GetParam());
        Second      = std::get<5>(GetParam());
        Millisecond = std::get<6>(GetParam());

        FixtureDateTime = DateTime(Year, Month, Day, Hour, Minute, Second, Millisecond);
        SHOW_DATETIME(FixtureDateTime);
    }
};

/// <summary>
///
/// </summary>
class DateTimeDateOnlyFormatFixture
    : public DateTimeConstructorFixture
    , public ::testing::WithParamInterface<std::tuple<int, int, int, string, string>>
{
public:
    int Year  = 0;
    int Month = 0;
    int Day   = 0;
    string Format;
    string ExpectedOutput;

    void SetUp() override
    {
        Year           = std::get<0>(GetParam());
        Month          = std::get<1>(GetParam());
        Day            = std::get<2>(GetParam());
        Format         = std::get<3>(GetParam());
        ExpectedOutput = std::get<4>(GetParam());

        FixtureDateTime = DateTime(Year, Month, Day);
        SHOW_DATETIME(FixtureDateTime);
    }
};

/// <summary>
///
/// </summary>
class DateTimeAdjustmentFixture
    : public DateTimeConstructorFixture
    , public ::testing::WithParamInterface<std::tuple<string, int, string>>
{
public:
    int IncrementValue = 0;
    DateTime ExpectedDateTime;

    void SetUp() override
    {
        IncrementValue = std::get<1>(GetParam());

        FixtureDateTime = DateTime::Parse(std::get<0>(GetParam()));
        SHOW_DATETIME(FixtureDateTime);

        Increment();
        SHOW_DATETIME_NAMED(FixtureDateTime, "Adjusted");

        ExpectedDateTime = DateTime::Parse(std::get<2>(GetParam()));
        SHOW_DATETIME(ExpectedDateTime);
    }

    virtual void Increment() = 0;
};

/// <summary>
///
/// </summary>
class DateTimeDateOnlyAddYearsFixture
    : public DateTimeAdjustmentFixture
{
public:
    void Increment() override
    {
        FixtureDateTime.AddYears(IncrementValue);
    }
};

/// <summary>
///
/// </summary>
class DateTimeDateOnlyAddMonthsFixture
    : public DateTimeAdjustmentFixture
{
public:
    void Increment() override
    {
        FixtureDateTime.AddMonths(IncrementValue);
    }
};

/// <summary>
///
/// </summary>
class DateTimeDateOnlyAddDaysFixture
    : public DateTimeAdjustmentFixture
{
public:
    void Increment() override
    {
        FixtureDateTime.AddDays(IncrementValue);
    }
};

/// <summary>
///
/// </summary>
class DateTimeDateOnlyAddHoursFixture
    : public DateTimeAdjustmentFixture
{
public:
    void Increment() override
    {
        FixtureDateTime.AddHours(IncrementValue);
    }
};

/// <summary>
///
/// </summary>
class DateTimeDateOnlyAddMinutesFixture
    : public DateTimeAdjustmentFixture
{
public:
    void Increment() override
    {
        FixtureDateTime.AddMinutes(IncrementValue);
    }
};

/// <summary>
///
/// </summary>
class DateTimeDateOnlyAddSecondsFixture
    : public DateTimeAdjustmentFixture
{
public:
    void Increment() override
    {
        FixtureDateTime.AddSeconds(IncrementValue);
    }
};

/// <summary>
///
/// </summary>
class DateTimeDateOnlyAddMillisecondsFixture
    : public DateTimeAdjustmentFixture
{
public:
    void Increment() override
    {
        FixtureDateTime.AddMilliseconds(IncrementValue);
    }
};


//--------------------------------------------------------------------------------
// Parameterized Tests

//------------------------------------------------------------
TEST_P(DateTimeDateOnlyConstructorFixture, CheckDateConstructor)
{
    // Assert
    ASSERT_EQ(Year, FixtureDateTime.GetYear());
    ASSERT_EQ(Month, FixtureDateTime.GetMonth());
    ASSERT_EQ(Day, FixtureDateTime.GetDay());
    ASSERT_EQ(0, FixtureDateTime.GetHour());
    ASSERT_EQ(0, FixtureDateTime.GetMinute());
    ASSERT_EQ(0, FixtureDateTime.GetSeconds());
    ASSERT_EQ(0, FixtureDateTime.GetMilliseconds());
    ASSERT_TRUE(FixtureDateTime.IsDateOnly());
}

INSTANTIATE_TEST_CASE_P(
    DateTimeConstructorTests,
    DateTimeDateOnlyConstructorFixture,
    ::testing::Values(
          std::make_tuple(DateTime::Base_Year, 1, 1)
        , std::make_tuple(DateTime::Epoch_Year, 1, 1)
        , std::make_tuple(1969, 12, 31)
        , std::make_tuple(2025, 10, 28)
        , std::make_tuple(2001, 1,  1)
        , std::make_tuple(2010, 5,  14)
        , std::make_tuple(1996, 12, 31)
        , std::make_tuple(1950, 11, 22)
        , std::make_tuple(2000, 2,  29)
        , std::make_tuple(1968, 8,  11)
    )
);

TEST_P(DateTimeDateOnlyParseFixture, CheckParseDate)
{
    // Assert
    ASSERT_EQ(Year, FixtureDateTime.GetYear());
    ASSERT_EQ(Month, FixtureDateTime.GetMonth());
    ASSERT_EQ(Day, FixtureDateTime.GetDay());
    ASSERT_EQ(0, FixtureDateTime.GetHour());
    ASSERT_EQ(0, FixtureDateTime.GetMinute());
    ASSERT_EQ(0, FixtureDateTime.GetSeconds());
    ASSERT_EQ(0, FixtureDateTime.GetMilliseconds());
    ASSERT_TRUE(FixtureDateTime.IsDateOnly());
}

INSTANTIATE_TEST_CASE_P(
    DateTimeConstructorTests,
    DateTimeDateOnlyParseFixture,
    ::testing::Values(
        std::make_tuple(to_string(DateTime::Base_Year) + "-01-01", DateTime::Base_Year, 1, 1)
        , std::make_tuple(to_string(DateTime::Epoch_Year) + "-01-01", DateTime::Epoch_Year, 1, 1)
        , std::make_tuple("1969-12-31", 1969, 12, 31)
        , std::make_tuple("2025-10-28", 2025, 10, 28)
        , std::make_tuple("2001-01-01", 2001, 1, 1)
        , std::make_tuple("2010-05-14", 2010, 5, 14)
        , std::make_tuple("1996-12-31", 1996, 12, 31)
        , std::make_tuple("1950-11-22", 1950, 11, 22)
        , std::make_tuple("2000-02-29", 2000, 2, 29)
        , std::make_tuple("1968-08-11", 1968, 8, 11)
    )
);

//------------------------------------------------------------
TEST_P(DateTimeDateAndTimeConstructorFixture, CheckDateConstructor)
{
    // Assert
    ASSERT_EQ(Year, FixtureDateTime.GetYear());
    ASSERT_EQ(Month, FixtureDateTime.GetMonth());
    ASSERT_EQ(Day, FixtureDateTime.GetDay());
    ASSERT_EQ(Hour, FixtureDateTime.GetHour());
    ASSERT_EQ(Minute, FixtureDateTime.GetMinute());
    ASSERT_EQ(Second, FixtureDateTime.GetSeconds());
    ASSERT_EQ(Millisecond, FixtureDateTime.GetMilliseconds());
    ASSERT_FALSE(FixtureDateTime.IsDateOnly());
}

INSTANTIATE_TEST_CASE_P(
    DateTimeConstructorTests,
    DateTimeDateAndTimeConstructorFixture,
    ::testing::Values(
          std::make_tuple(DateTime::Base_Year, 1, 1, 2, 3, 4, 567)
        , std::make_tuple(DateTime::Epoch_Year, 1, 1, 12, 00, 00, 000)
        , std::make_tuple(2025, 10, 28, 17, 45, 03, 123)
        , std::make_tuple(2001, 1, 1, 3, 00, 01, 000)
        , std::make_tuple(2010, 5, 14, 11, 11, 11, 111)
        , std::make_tuple(1996, 12, 31, 23, 59, 59, 999)
        , std::make_tuple(1950, 11, 22, 19, 30, 05, 783)
        , std::make_tuple(2000, 2, 29, 15, 45, 00, 000)
        , std::make_tuple(1968, 8, 11, 11, 30, 05, 654)
    )
);

//------------------------------------------------------------
TEST_P(DateTimeDateOnlyFormatFixture, CheckFormattedDate)
{
    // Assert
    ASSERT_EQ(FixtureDateTime.ToString(Format), ExpectedOutput);
}

INSTANTIATE_TEST_CASE_P(
    DateTimeConstructorTests,
    DateTimeDateOnlyFormatFixture,
    ::testing::Values(
          std::make_tuple(DateTime::Base_Year, 1, 1, DateTime::Formats::Date_Default, "Mon, Jan  1 " + to_string(DateTime::Base_Year))
        , std::make_tuple(DateTime::Epoch_Year, 1, 1, DateTime::Formats::Date_Default, "Thu, Jan  1 " + to_string(DateTime::Epoch_Year))
    )
);

//------------------------------------------------------------
TEST_P(DateTimeDateOnlyAddYearsFixture, AddYearsToDate)
{
    // Assert
    ASSERT_EQ(ExpectedDateTime, FixtureDateTime);
}

INSTANTIATE_TEST_CASE_P(
    DateTimeManipulationTests,
    DateTimeDateOnlyAddYearsFixture,
    ::testing::Values(
          std::make_tuple("2025-01-01", 10, "2035-01-01")
        , std::make_tuple("2025-01-01", -10, "2015-01-01")
    )
);

//------------------------------------------------------------
TEST_P(DateTimeDateOnlyAddMonthsFixture, AddMonthsToDate)
{
    // Assert
    ASSERT_EQ(ExpectedDateTime, FixtureDateTime);
}

INSTANTIATE_TEST_CASE_P(
    DateTimeManipulationTests,
    DateTimeDateOnlyAddMonthsFixture,
    ::testing::Values(
          std::make_tuple("2025-01-01",  10, "2025-11-01")
        , std::make_tuple("2025-01-01", -10, "2024-03-01")
        , std::make_tuple("2025-01-01",  20, "2026-09-01")
        , std::make_tuple("2025-01-01", -20, "2023-05-01")
    )
);

//------------------------------------------------------------
//TEST_P(DateTimeDateOnlyAddYearsFixture, AddDaysToDate)
//{
//    ASSERT_EQ(FixtureDateTime.GetYear(), Year + AddValue);
//    ASSERT_EQ(FixtureDateTime.GetMonth(), Month);
//    ASSERT_EQ(FixtureDateTime.GetDay(), Day);
//}
//
//INSTANTIATE_TEST_CASE_P(
//    DateTimeManipulationTests,
//    DateTimeDateOnlyAddDaysFixture,
//    ::testing::Values(
//        std::make_tuple(DateTime::Base_Year, 1, 1, 10)
//        , std::make_tuple(DateTime::Epoch_Year, 1, 1, 20)
//        , std::make_tuple(2023, 10, 28, 5)
//    )
//);
