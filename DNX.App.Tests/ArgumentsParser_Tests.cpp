#include "pch.h"
#include "../DNX.App/Arguments.h"
#include "../DNX.App/ArgumentsParser.h"
#include "Arguments1.h"
#include "Arguments2.h"
#include "Arguments3.h"
#include "TestHelper.h"

// ReSharper disable CppClangTidyPerformanceUnnecessaryCopyInitialization
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyMiscUseAnonymousNamespace
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppStringLiteralToCharPointerConversion
// ReSharper disable CppClangTidyClangDiagnosticWritableStrings

using namespace std;
using namespace DNX::App;

#define TEST_GROUP ArgumentsParser

TEST(TEST_GROUP, ParseArguments_single_positional_arguments_assigns_correctly)
{
    constexpr auto argc = 2;
    char* argv[argc] = {
        "",
        "bob",
    };

    Arguments1 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_TRUE(arguments.IsValid());
    EXPECT_EQ("bob", arguments.GetMessageText());
}

TEST(TEST_GROUP, ParseArguments_single_positional_argument_with_shortname_option_after_and_switch_assigns_correctly)
{
    constexpr auto argc = 5;
    char* argv[argc] = {
        "",
        "bob",
        "-t",
        "5",
        "-x",
    };

    Arguments1 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_TRUE(arguments.IsValid());
    EXPECT_EQ("bob", arguments.GetMessageText());
    EXPECT_EQ(5, arguments.GetTimeoutSeconds());
    EXPECT_EQ(true, arguments.IsDebug());
}

TEST(TEST_GROUP, ParseArguments_single_positional_argument_with_shortname_option_before_and_switch_assigns_correctly)
{
    constexpr auto argc = 5;
    char* argv[argc] = {
        "",
        "-t",
        "5",
        "bob",
        "-x",
    };

    Arguments1 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_TRUE(arguments.IsValid());
    EXPECT_EQ("bob", arguments.GetMessageText());
    EXPECT_EQ(5, arguments.GetTimeoutSeconds());
    EXPECT_EQ(true, arguments.IsDebug());
}

TEST(TEST_GROUP, ParseArguments_single_positional_argument_with_shortname_arguments_before_and_after_and_switch_assigns_correctly)
{
    constexpr auto argc = 7;
    char* argv[argc] = {
        "",
        "-t",
        "5",
        "bob",
        "-x",
        "-s",
        "500",
    };

    Arguments1 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_TRUE(arguments.IsValid());
    EXPECT_EQ("bob", arguments.GetMessageText());
    EXPECT_EQ(5, arguments.GetTimeoutSeconds());
    EXPECT_EQ(500, arguments.GetSleepMilliseconds());
    EXPECT_EQ(true, arguments.IsDebug());
}

TEST(TEST_GROUP, IsValid_arguments_without_any_required_arguments_returns_successfully)
{
    constexpr auto argc = 1;
    char* argv[argc] = {
        "",
    };

    Arguments1 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_TRUE(arguments.IsValid());
}

TEST(TEST_GROUP, IsValid_arguments_with_required_arguments_returns_successfully)
{
    constexpr auto argc = 1;
    char* argv[argc] = {
        "",
    };

    Arguments2 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_FALSE(arguments.IsValid());
}

TEST(TEST_GROUP, IsValid_arguments_with_option_shortname_without_value_returns_successfully)
{
    constexpr auto argc = 2;
    char* argv[argc] = {
        "",
        "-t",
    };

    Arguments1 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_FALSE(arguments.IsValid());
}

TEST(TEST_GROUP, IsValid_arguments_with_option_longname_without_value_returns_successfully)
{
    constexpr auto argc = 2;
    char* argv[argc] = {
        "",
        "--timeout",
    };

    Arguments1 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_FALSE(arguments.IsValid());
}

TEST(TEST_GROUP, IsValid_arguments_with_option_longname_with_invalid_value_returns_successfully)
{
    constexpr auto argc = 3;
    char* argv[argc] = {
        "",
        "--timeout",
        "abc",
    };

    Arguments1 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_FALSE(arguments.IsValid());
}

TEST(TEST_GROUP, IsValid_arguments_with_non_required_string_option_returns_successfully)
{
    constexpr auto argc = 2;
    char* argv[argc] = {
        "",
        "bob",
    };

    Arguments3 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_TRUE(arguments.IsValid());
    EXPECT_EQ(arguments.GetMessageText(), "bob");
    EXPECT_EQ(arguments.GetFormat(), "");
}

TEST(TEST_GROUP, IsValid_more_parameters_supplied_than_are_expected_returns_successfully)
{
    constexpr auto argc = 3;
    char* argv[argc] = {
        "",
        "bob",
        "dave",
    };

    Arguments3 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_FALSE(arguments.IsValid());
}
