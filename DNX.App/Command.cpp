#include "stdafx.h"
#include "Command.h"
#include "../DNX.Utils/StringUtils.h"

// ReSharper disable CppInconsistentNaming

using namespace std;
using namespace DNX::App;

EmptyCommand Command::_empty_command = EmptyCommand();

Command::Command(
    Arguments* arguments,
    const string& name,
    const string& description
)
{
    if (arguments == nullptr)
        throw exception("Arguments must be valid");
    if (StringUtils::Trim(name).empty())
        throw exception("name cannot be empty or blank");

    _arguments   = arguments;
    _name        = name;
    _description = description;
}

Command::Command()
{
    _arguments   = &Arguments::Empty();
    _name        = "";
    _description = "";
}

bool Command::IsEmpty() const
{
    return _name.empty();
}
Arguments& Command::GetArguments() const
{
    return *_arguments;
}
string Command::GetName() const
{
    return _name;
}
string Command::GetDescription() const
{
    return _description;
}

void Command::Execute()
{
}
