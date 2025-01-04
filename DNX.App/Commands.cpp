#include "stdafx.h"
#include "Command.h"
#include "Commands.h"
#include <complex>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppTooWideScope
// ReSharper disable CppClangTidyClangDiagnosticImplicitIntConversion

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;

CommandArguments& Commands::GetArguments()
{
    return _arguments;
}

void Commands::AddCommand(Arguments* arguments, const string& name, const string& description)
{
    const auto command = Command(arguments, name, description);

    _commands[command.GetName()] = command;
}

list<Command> Commands::GetCommands() const
{
    list<Command> commands;

    for (auto iter = _commands.begin(); iter != _commands.end(); ++iter)
    {
        auto command = iter->second;
        commands.emplace_back(command);
    }

    return commands;
}

Command& Commands::GetCommandByName(const string& commandName)
{
    const auto iter = _commands.find(commandName);
    if (iter == _commands.end())
        return Command::Empty();

    return iter->second;
}

list<string> Commands::GetErrors() const
{
    return _arguments.GetErrors();
}

bool Commands::IsValid() const
{
    return _arguments.IsValid();
}
