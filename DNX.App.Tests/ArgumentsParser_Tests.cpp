#include "pch.h"
#include "../DNX.App/Arguments.h"
#include "../DNX.App/ArgumentsParser.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/PathUtils.h"
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
    char* argv[] = {
        const_cast<char*>(""),
        const_cast<char*>("bob"),
    };
    constexpr auto argc = size(argv);

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
    char* argv[] = {
        const_cast<char*>(""),
        const_cast<char*>("bob"),
        const_cast<char*>("-t"),
        const_cast<char*>("5"),
        const_cast<char*>("-x"),
    };
    constexpr auto argc = size(argv);

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
    char* argv[] = {
        const_cast<char*>(""),
        const_cast<char*>("-t"),
        const_cast<char*>("5"),
        const_cast<char*>("bob"),
        const_cast<char*>("-x"),
    };
    constexpr auto argc = size(argv);

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
    char* argv[] = {
        const_cast<char*>(""),
        const_cast<char*>("-t"),
        const_cast<char*>("5"),
        const_cast<char*>("bob"),
        const_cast<char*>("-x"),
        const_cast<char*>("-s"),
        const_cast<char*>("500"),
    };
    constexpr auto argc = size(argv);

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

TEST(TEST_GROUP, ParseArguments_parse_a_file_of_arguments_correctly)
{
    const auto fileName = PathUtils::GetTempFileName("args");

    const string quote = "\"";
    const auto message_text = "Some text";
    constexpr auto timeout = 600;

    auto lines = list<string>();
    lines.emplace_back(quote + message_text + quote);
    lines.emplace_back("-t " + to_string(timeout));
    FileUtils::WriteAllLines(fileName, lines);

    auto args = list<string>();
    args.emplace_back("@" + fileName);

    Arguments1 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, args);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_EQ(arguments.GetMessageText(), message_text);
    EXPECT_EQ(arguments.GetTimeoutSeconds(), timeout);
}

TEST(TEST_GROUP, IsValid_arguments_without_any_required_arguments_returns_successfully)
{
    char* argv[] = {
        const_cast<char*>(""),
    };
    constexpr auto argc = size(argv);

    Arguments1 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_TRUE(arguments.IsValid());
}

TEST(TEST_GROUP, IsValid_arguments_with_required_arguments_returns_successfully)
{
    char* argv[] = {
        const_cast<char*>(""),
    };
    constexpr auto argc = size(argv);

    Arguments2 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_FALSE(arguments.IsValid());
}

TEST(TEST_GROUP, IsValid_arguments_with_option_shortname_without_value_returns_successfully)
{
    char* argv[] = {
        const_cast<char*>(""),
        const_cast<char*>("-t"),
    };
    constexpr auto argc = size(argv);

    Arguments1 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_FALSE(arguments.IsValid());
}

TEST(TEST_GROUP, IsValid_arguments_with_option_longname_without_value_returns_successfully)
{
    char* argv[] = {
        const_cast<char*>(""),
        const_cast<char*>("--timeout"),
    };
    constexpr auto argc = size(argv);

    Arguments1 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_FALSE(arguments.IsValid());
}

TEST(TEST_GROUP, IsValid_arguments_with_option_longname_with_invalid_value_returns_successfully)
{
    char* argv[] = {
        const_cast<char*>(""),
        const_cast<char*>("--timeout"),
        const_cast<char*>("abc"),
    };
    constexpr auto argc = size(argv);

    Arguments1 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_FALSE(arguments.IsValid());
}

TEST(TEST_GROUP, IsValid_arguments_with_non_required_string_option_returns_successfully)
{
    char* argv[] = {
        const_cast<char*>(""),
        const_cast<char*>("bob"),
    };
    constexpr auto argc = size(argv);

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
    char* argv[] = {
        const_cast<char*>(""),
        const_cast<char*>("bob"),
        const_cast<char*>("dave"),
    };
    constexpr auto argc = size(argv);

    Arguments3 arguments;

    // Act
    ArgumentsParser::ParseArguments(arguments, argc, argv);
    TestHelper::ShowErrors(arguments);

    // Assert
    EXPECT_FALSE(arguments.IsValid());
}
