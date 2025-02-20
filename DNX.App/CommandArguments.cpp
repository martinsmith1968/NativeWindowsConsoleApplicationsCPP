#include "stdafx.h"
#include "CommandArguments.h"

// ReSharper disable CppInconsistentNaming

using namespace std;
using namespace DNX::App;

void CommandArguments::BuildFromCommands(list<Command> commands)
{
    Reset();
    AddFileOverrideArguments();

    list<string> command_name_value_list;
    for (auto iter = commands.begin(); iter != commands.end(); ++iter)
    {
        command_name_value_list.emplace_back(iter->GetName());
    }

    AddParameter(ValueType::STRING, 1, CommandNameLongName, "", CommandNameDescription, true, command_name_value_list);
}

string CommandArguments::GetCommandNameValue()
{
    return GetArgumentValue(CommandNameLongName);
}
