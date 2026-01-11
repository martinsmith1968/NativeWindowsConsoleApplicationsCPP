#include "pch.h"
#include "../DNX.App/AppDetails.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceUnnecessaryCopyInitialization
// ReSharper disable CppClangTidyPerformanceAvoidEndl

using namespace std;
using namespace DNX::App;

#define TEST_GROUP AppDetails

TEST(TEST_GROUP, GetApplicationName_returns_the_correct_value)
{
    EXPECT_EQ("DNX.App.Tests", AppDetails::GetApplicationName());
}

TEST(TEST_GROUP, GetDefaultArgumentsFileName_returns_the_correct_value)
{
    const string expected = "DNX.App.Tests." + AppDetails::GetDefaultArgumentsFileExtension();

    // Act
    const auto filename = AppDetails::GetDefaultArgumentsFileName();
    const auto result = filename.substr(filename.length() - expected.length());

    // Assert
    cout << "GetDefaultArgumentsFileName: " << filename << endl;
    cout << "GetDefaultArgumentsFileName result: " << result << endl;
    EXPECT_EQ(expected, result);
}

TEST(TEST_GROUP, GetLocalArgumentsFileName_returns_the_correct_value)
{
    const string expected = "DNX.App.Tests." + AppDetails::GetDefaultArgumentsFileExtension();

    // Act
    const auto filename = AppDetails::GetLocalArgumentsFileName();
    const auto result = filename.substr(filename.length() - expected.length());

    // Assert
    cout << "GetLocalArgumentsFileName: " << filename << endl;
    cout << "GetLocalArgumentsFileName result: " << result << endl;
    EXPECT_EQ(expected, result);
    EXPECT_NE(result, AppDetails::GetDefaultArgumentsFileName());
}

TEST(TEST_GROUP, GetHeaderLine_with_all_details_returns_the_correct_value)
{
    const string app_name = "MyAppName";
    const string app_description = "My App";
    const string app_copyright = "Copyright (c) 2024 My Company";
    const string app_version = "1.2.3.4";

    auto app_details = AppDetails();
    app_details.Name = app_name;
    app_details.Description = app_description;
    app_details.Copyright = app_copyright;
    app_details.Version = app_version;

    auto expected = app_name + " v" + app_version + " - " + app_description;

    // Act
    auto result = app_details.GetHeaderLine();

    // Assert
    EXPECT_EQ(expected, result);
}

TEST(TEST_GROUP, GetHeaderLine_with_all_details_except_description_returns_the_correct_value)
{
    const string app_name = "MyAppName";
    const string app_copyright = "Copyright (c) 2024 My Company";
    const string app_version = "1.2.3.4";

    auto app_details = AppDetails();
    app_details.Name = app_name;
    app_details.Copyright = app_copyright;
    app_details.Version = app_version;

    const auto expected = app_name + " v" + app_version;

    // Act
    const auto result = app_details.GetHeaderLine();

    // Assert
    EXPECT_EQ(expected, result);
}

TEST(TEST_GROUP, GetHeaderLine_with_all_details_except_version_and_description_returns_the_correct_value)
{
    const string app_name = "MyAppName";
    const string app_copyright = "Copyright (c) 2024 My Company";

    auto app_details = AppDetails();
    app_details.Name = app_name;
    app_details.Copyright = app_copyright;

    const auto expected = app_name;

    // Act
    const auto result = app_details.GetHeaderLine();

    // Assert
    EXPECT_EQ(expected, result);
}
