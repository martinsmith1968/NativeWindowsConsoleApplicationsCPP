#include "stdafx.h"
#include "CommandArguments.h"
#include "CommandsParser.h"
#include "ArgumentsParser.h"
#include <string>

#include "../DNX.Utils/ListUtils.h"
#include "../DNX.Utils/StringUtils.h"

// ReSharper disable CppClangTidyModernizePassByValue
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppTooWideScopeInitStatement

//-----------------------------------------------------------------------------
// Public usage methods
CommandsParser::CommandsParser(Commands& commands, const AppDetails& app_details, const ParserConfig& parser_config)
    :
    _commands(commands),
    _parser_config(parser_config),
    _app_details(app_details)
{
}

Command& CommandsParser::Parse(int argc, char* argv[]) const
{
    auto commands = list<Command>();
    for (Command& command : _commands.GetCommands())
        commands.emplace_back(command);

    auto& command_arguments = _commands.GetArguments();
    command_arguments.BuildFromCommands(commands);

    ParserConfig commands_parser_config = _parser_config;
    commands_parser_config.SetIgnoreAdditionalArguments(true);

    const auto command_parser = ArgumentsParser(command_arguments, _app_details, commands_parser_config);
    command_parser.Parse(argc, argv);

    const auto selected_command_name = command_arguments.GetCommandNameValue();
    if (!selected_command_name.empty())
    {
        auto& selected_command = _commands.GetCommandByName(selected_command_name);
        if (selected_command.IsEmpty())
            command_arguments.AddError("Invalid or unknown command: " + selected_command_name);

        // Filter remaining arguments on to be parsed
        const auto refined_arguments = RefineCommandArguments(ListUtils::ToList(argc, argv, 1), selected_command.GetName());
        const auto arguments_parser = ArgumentsParser(selected_command.GetArguments(), _app_details, _parser_config);
        arguments_parser.Parse(refined_arguments);

        return selected_command;
    }

    return Command::Empty();
}

list<string> CommandsParser::RefineCommandArguments(const list<string>& arguments, const string& commandName)
{
    auto removed = false;
    list<string> result;
    for (auto& argument : arguments)
    {
        if (!removed && StringUtils::ToLower(argument) == StringUtils::ToLower(commandName))
        {
            removed = true;
            continue;
        }

        result.emplace_back(argument);
    }

    return result;
}

//-----------------------------------------------------------------------------
// Static Public methods
Command& CommandsParser::ParseCommands(Commands& commands, int argc, char* argv[], const AppDetails& app_details, const ParserConfig& parser_config)
{
    const auto parser = CommandsParser(commands, app_details, parser_config);
    auto& command = parser.Parse(argc, argv);
    return command;
}
