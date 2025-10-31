#include "pch.h"

#include "Commands1.h"
#include "TestHelper.h"
#include "../DNX.App/Commands.h"
#include "../DNX.App/CommandsParser.h"

using namespace std;
using namespace DNX::App;

// ReSharper disable CppClangTidyMiscUseAnonymousNamespace
// ReSharper disable CppInconsistentNaming

#define TEST_GROUP CommandsParser

TEST(TEST_GROUP, ParseCommands_no_command_parses_correctly)
{
    char* argv[] = {
        const_cast<char*>(""),
    };
    constexpr auto argc = size(argv);

    Commands1 commands;

    // Act
    const auto& result = CommandsParser::ParseCommands(commands, argc, argv);
    TestHelper::ShowErrors(commands);

    // Assert
    EXPECT_TRUE(result.IsEmpty());
    EXPECT_FALSE(commands.IsValid());
}

TEST(TEST_GROUP, ParseCommands_invalid_command_parses_correctly)
{
    char* argv[] = {
        const_cast<char*>(""),
        const_cast<char*>("bob"),
    };
    constexpr auto argc = size(argv);

    Commands1 commands;

    // Act
    const auto& result = CommandsParser::ParseCommands(commands, argc, argv);
    TestHelper::ShowErrors(commands);

    // Assert
    EXPECT_TRUE(result.IsEmpty());
    EXPECT_FALSE(commands.IsValid());
}

TEST(TEST_GROUP, ParseCommands_valid_command_without_arguments_parses_correctly)
{
    char* argv[] = {
        const_cast<char*>(""),
        const_cast<char*>("c2"),
    };
    constexpr auto argc = size(argv);

    Commands1 commands;

    // Act
    const auto& result = CommandsParser::ParseCommands(commands, argc, argv);
    TestHelper::ShowErrors(commands);

    // Assert
    EXPECT_FALSE(result.IsEmpty());
    EXPECT_TRUE(commands.IsValid());
    EXPECT_EQ(result.GetName(), "c2");

    auto result_arguments = dynamic_cast<AppArgumentsC2&>(result.GetArguments());
    EXPECT_FALSE(result_arguments.GetFlagW());
}

TEST(TEST_GROUP, ParseCommands_valid_command_with_optional_arguments_parses_correctly)
{
    char* argv[] = {
        const_cast<char*>(""),
        const_cast<char*>("c2"),
        const_cast<char*>("-w"),
    };
    constexpr auto argc = size(argv);

    Commands1 commands;

    // Act
    const auto& result = CommandsParser::ParseCommands(commands, argc, argv);
    TestHelper::ShowErrors(commands);

    // Assert
    EXPECT_FALSE(result.IsEmpty());
    EXPECT_TRUE(commands.IsValid());
    EXPECT_EQ(result.GetName(), "c2");

    auto result_arguments = dynamic_cast<AppArgumentsC2&>(result.GetArguments());
    EXPECT_TRUE(result_arguments.GetFlagW());
}

TEST(TEST_GROUP, ParseCommands_valid_command_without_required_arguments_parses_correctly)
{
    char* argv[] = {
        const_cast<char*>(""),
        const_cast<char*>("c1"),
        const_cast<char*>("-v"),
    };
    constexpr auto argc = size(argv);

    Commands1 commands;

    // Act
    const auto& result = CommandsParser::ParseCommands(commands, argc, argv);
    TestHelper::ShowErrors(result);

    // Assert
    EXPECT_FALSE(result.IsEmpty());
    EXPECT_FALSE(result.GetArguments().IsEmpty());
    EXPECT_FALSE(result.GetArguments().IsValid());
    EXPECT_EQ(result.GetName(), "c1");

    auto result_arguments = dynamic_cast<AppArgumentsC1&>(result.GetArguments());
    TestHelper::ShowErrors(result_arguments);
    EXPECT_TRUE(result_arguments.GetFlagV());
}

TEST(TEST_GROUP, ParseCommands_valid_command_with_required_arguments_parses_correctly)
{
    char* argv[] = {
        const_cast<char*>(""),
        const_cast<char*>("c1"),
        const_cast<char*>("-v"),
        const_cast<char*>("myname"),
    };
    constexpr auto argc = size(argv);

    Commands1 commands;

    // Act
    const auto& result = CommandsParser::ParseCommands(commands, argc, argv);
    TestHelper::ShowErrors(result);

    // Assert
    EXPECT_FALSE(result.IsEmpty());
    EXPECT_FALSE(result.GetArguments().IsEmpty());
    EXPECT_TRUE(result.GetArguments().IsValid());
    EXPECT_EQ(result.GetName(), "c1");

    auto result_arguments = dynamic_cast<AppArgumentsC1&>(result.GetArguments());
    EXPECT_EQ(result_arguments.GetItemName(), "myname");
    EXPECT_TRUE(result_arguments.GetFlagV());
}
