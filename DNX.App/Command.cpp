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
    const string& description,
    const int sequence
)
{
    if (arguments == nullptr)
        throw exception("Arguments must be valid");
    if (StringUtils::Trim(name).empty())
        throw exception("name cannot be empty or blank");

    m_arguments   = arguments;
    m_name        = name;
    m_description = description;
    m_sequence    = sequence;
}

Command::Command()
{
    m_arguments   = &Arguments::Empty();
    m_name        = "";
    m_description = "";
    m_sequence    = 0;
}

bool Command::IsEmpty() const
{
    return m_name.empty();
}
Arguments& Command::GetArguments() const
{
    return *m_arguments;
}
string Command::GetName() const
{
    return m_name;
}
string Command::GetDescription() const
{
    return m_description;
}
int Command::GetSequence() const
{
    return m_sequence;
}

void Command::Execute()
{
}
