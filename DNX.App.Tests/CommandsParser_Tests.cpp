#include "pch.h"

#include "Commands1.h"
#include "TestHelper.h"
#include "../DNX.App/Commands.h"
#include "../DNX.App/CommandsParser.h"
#include "../SimpleTimer/AppArguments.h"

using namespace std;
using namespace DNX::App;

// ReSharper disable CppClangTidyMiscUseAnonymousNamespace
// ReSharper disable CppInconsistentNaming

#define TEST_GROUP CommandsParser

TEST(TEST_GROUP, ParseCommands_no_command_parses_correctly)
{
    constexpr auto argc = 1;
    char* argv[argc] = {
        "",
    };

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
    constexpr auto argc = 2;
    char* argv[argc] = {
        "",
        "bob",
    };

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
    constexpr auto argc = 2;
    char* argv[argc] = {
        "",
        "c2",
    };

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
    constexpr auto argc = 3;
    char* argv[argc] = {
        "",
        "c2",
        "-w",
    };

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
    constexpr auto argc = 3;
    char* argv[argc] = {
        "",
        "c1",
        "-v",
    };

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
    constexpr auto argc = 4;
    char* argv[argc] = {
        "",
        "c1",
        "-v",
        "myname",
    };

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
