#include "pch.h"

#include <thread>

#include "../DNX.Utils/DateTime.h"
#include "../DNX.Utils/StringUtils.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppTooWideScope
// ReSharper disable CppClangTidyMiscUseAnonymousNamespace

#define TEST_GROUP DateTime

#define SHOW_DATETIME(var) ShowDateTime(var, #var)

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
    cout << "      Ms: " << dateTime.GetMilliseconds() << endl;
}

static void ShowDuration(const chrono::system_clock::duration duration)
{
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration - hours);
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration - hours - minutes);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration - hours - minutes - seconds);
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration - hours - minutes - seconds - milliseconds);
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration - hours - minutes - seconds - milliseconds - microseconds);

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
    auto now = std::chrono::system_clock::now();

    time_t tnow = std::chrono::system_clock::to_time_t(now);
    tm* date = std::localtime(&tnow);
    date->tm_hour = 0;
    date->tm_min = 0;
    date->tm_sec = 0;

    auto midnight = std::chrono::system_clock::from_time_t(std::mktime(date));

    return now - midnight;
}

//--------------------------------------------------------------------------------
// Test Fixtures
class DateTimeDateOnlyConstructorFixture :public ::testing::TestWithParam<std::tuple<int, int, int>>
{
public:
    DateTime FixtureDateTime;
    int Year;
    int Month;
    int Day;

    void SetUp() override
    {
        Year = std::get<0>(GetParam());
        Month = std::get<1>(GetParam());
        Day = std::get<2>(GetParam());

        FixtureDateTime = DateTime(Year, Month, Day);
        SHOW_DATETIME(FixtureDateTime);
    }
};

class DateTimeDateAndTimeConstructorFixture :public ::testing::TestWithParam<std::tuple<int, int, int, int, int, int, int>>
{
public:
    DateTime FixtureDateTime;
    int Year;
    int Month;
    int Day;
    int Hour;
    int Minute;
    int Second;
    int Millisecond;

    void SetUp() override
    {
        Year = std::get<0>(GetParam());
        Month = std::get<1>(GetParam());
        Day = std::get<2>(GetParam());
        Hour = std::get<3>(GetParam());
        Minute = std::get<4>(GetParam());
        Second = std::get<5>(GetParam());
        Millisecond = std::get<6>(GetParam());

        FixtureDateTime = DateTime(Year, Month, Day, Hour, Minute, Second, Millisecond);
        SHOW_DATETIME(FixtureDateTime);
    }
};

//--------------------------------------------------------------------------------
// Tests
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
    //EXPECT_NE(result1.GetMilliseconds(), result2.GetMilliseconds());
}

TEST_P(DateTimeDateOnlyConstructorFixture, CheckDateConstructor)
{
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
        , std::make_tuple(2025, 10, 28)
        , std::make_tuple(2001, 1,  1)
        , std::make_tuple(2010, 5,  14)
        , std::make_tuple(1996, 12, 31)
        , std::make_tuple(1950, 11, 22)
        , std::make_tuple(2000, 2,  29)
        , std::make_tuple(1968, 8,  11)
    )
);

TEST_P(DateTimeDateAndTimeConstructorFixture, CheckDateConstructor)
{
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
