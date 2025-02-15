#include "stdafx.h"
#include "Argument.h"
#include "Arguments.h"
#include <complex>

#include "../DNX.Utils/ListUtils.h"
#include "../DNX.Utils/StringUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppTooWideScope
// ReSharper disable CppClangTidyClangDiagnosticImplicitIntConversion
// ReSharper disable CppTooWideScopeInitStatement

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;

Arguments Arguments::_empty_arguments = Arguments();

//-----------------------------------------------------------------------------
// Internal methods
void Arguments::PostParseValidate()
{
}

void Arguments::AddArgumentComplete(
    const ArgumentType argumentType,
    const ValueType valueType,
    const string& shortName,
    const string& longName,
    const string& defaultValue,
    const string& description,
    const bool required,
    const int position,
    const list<string>& valueList
)
{
    if (shortName.empty() && argumentType != ArgumentType::PARAMETER)
        throw exception("ShortName is required");
    if (longName.empty())
        throw exception("LongName is required");
    if (!defaultValue.empty() && !valueList.empty())
    {
        if (!ListUtils::Exists(valueList, defaultValue))
            throw exception("Default Value is not one of the constrained values");
    }

    const auto& existingArgument = GetArgumentByName(shortName);
    if (!existingArgument.IsEmpty())
        throw exception((string("Argument already exists: ") + shortName).c_str());

    uint8_t realPosition = static_cast<uint8_t>(position);
    string realShortName = shortName;

    if (argumentType == ArgumentType::PARAMETER)
    {
        realPosition = static_cast<uint8_t>(GetArgumentsByType(ArgumentType::PARAMETER).size() + 1);
        realShortName = "";

        if (longName.empty())
        {
            throw exception((string("Parameter must have a long name: ") + to_string(realPosition)).c_str());
        }
    }
    else
    {
        if (position == 0)
        {
            const auto types = { ArgumentType::OPTION, ArgumentType::SWITCH };
            realPosition = static_cast<int>(GetArgumentsByTypes(types).size() + 1);
        }
    }

    const auto argument = Argument(argumentType, valueType, realPosition, realShortName, longName, description, defaultValue, required, valueList);

    _arguments[argument.GetLongName()] = argument;
}

void Arguments::AddNote(const string& note)
{
    _notes.push_back(note);
}

void Arguments::AddArgument(
    const ArgumentType argumentType,
    const ValueType valueType,
    const string& shortName,
    const string& longName,
    const string& defaultValue,
    const string& description,
    const bool required,
    const int position,
    const list<string>& valueList
)
{
    AddArgumentComplete(argumentType, valueType, shortName, longName, defaultValue, description, required, position, valueList);
}

void Arguments::AddParameter(
    const ValueType valueType,
    const int position,
    const string& longName,
    const string& defaultValue,
    const string& description,
    const bool required,
    const list<string>& valueList
)
{
    int realPosition = position;
    if (position <= 0)
    {
        const auto types = { ArgumentType::PARAMETER };
        realPosition = static_cast<int>(GetArgumentsByTypes(types).size() + 1);
    }

    AddArgument(ArgumentType::PARAMETER, valueType, "", longName, defaultValue, description, required, realPosition, valueList);
}

void Arguments::AddOption(
    const ValueType valueType,
    const string& shortName,
    const string& longName,
    const string& defaultValue,
    const string& description,
    const bool required,
    const int position,
    const list<string>& valueList
)
{
    int realPosition = position;
    if (position <= 0)
    {
        const auto types = { ArgumentType::OPTION, ArgumentType::SWITCH };
        realPosition = static_cast<int>(GetArgumentsByTypes(types).size() + 1);
    }

    AddArgument(ArgumentType::OPTION, valueType, shortName, longName, defaultValue, description, required, realPosition, valueList);
}

void Arguments::AddSwitch(
    const string& shortName,
    const string& longName,
    const string& defaultValue,
    const string& description,
    const bool required,
    const int position
)
{
    if (!ValueConverter::IsBool(defaultValue))
        throw exception((string("Default Value is not a BOOL: ") + defaultValue).c_str());

    int realPosition = position;
    if (position <= 0)
    {
        const auto types = { ArgumentType::OPTION, ArgumentType::SWITCH };
        realPosition = static_cast<int>(GetArgumentsByTypes(types).size() + 1);
    }

    AddArgument(ArgumentType::SWITCH, ValueType::BOOL, shortName, longName, defaultValue, description, required, realPosition);
}

void Arguments::AddError(const string& text)
{
    _errors.push_back(text);
}

Argument& Arguments::GetArgumentByLongName(const string& longName)
{
    const auto found = _arguments.find(longName);
    if (found == _arguments.end())
        return Argument::Empty();

    return found->second;
}

Argument& Arguments::GetArgumentByShortName(const string& shortName)
{
    for (auto iter = _arguments.begin(); iter != _arguments.end(); ++iter)
    {
        if (iter->second.GetShortName() == shortName)
        {
            return iter->second;
        }
    }

    return Argument::Empty();
}

Argument& Arguments::GetArgumentByName(const string& name)
{
    auto& property = GetArgumentByShortName(name);
    if (!property.IsEmpty())
        return property;

    return GetArgumentByLongName(name);
}

Argument& Arguments::GetParameterAtPosition(const int position)
{
    for (auto iter = _arguments.begin(); iter != _arguments.end(); ++iter)
    {
        if (iter->second.GetArgumentType() == ArgumentType::PARAMETER && iter->second.GetPosition() == position)
        {
            return iter->second;
        }
    }

    return Argument::Empty();
}

list<Argument> Arguments::GetRequiredArguments() const
{
    list<Argument> filtered;

    auto typeList = GetArguments();
    for (auto iter = typeList.begin(); iter != typeList.end(); ++iter)
    {
        if (iter->GetRequired())
        {
            filtered.push_back(*iter);
        }
    }

    return filtered;
}

string Arguments::GetArgumentValue(const string& name)
{
    const auto& argument = GetArgumentByName(name);
    if (argument.IsEmpty())
        throw exception((string("Unknown Argument: ") + name).c_str());

    const auto found = _values.find(argument.GetLongName());
    if (found != _values.end())
        return found->second;

    return argument.GetDefaultValue();
}

bool Arguments::GetSwitchValue(const string& name)
{
    const auto& argument = GetArgumentByName(name);
    if (argument.IsEmpty())
        throw exception((string("Unknown Argument: ") + name).c_str());
    if (argument.GetArgumentType() != ArgumentType::SWITCH)
        throw exception((string("Argument is not a switch: ") + name).c_str());

    const auto found = _values.find(argument.GetLongName());
    if (found != _values.end())
        return ValueConverter::ToBool(found->second);

    return ValueConverter::ToBool(argument.GetDefaultValue());
}

void Arguments::SetArgumentValue(const string& name, const string& value)
{
    const auto& argument = GetArgumentByName(name);
    if (argument.IsEmpty())
        throw exception((string("Unknown Argument: ") + name).c_str());

    auto actual_value = value;

    const auto values = argument.GetValueList();
    if (!values.empty())
    {
        if (!ListUtils::Exists(values, actual_value))
        {
            actual_value = ListUtils::Find(values, value, false);
            if (actual_value.empty())
                AddError(argument.GetLongName() + string(": Invalid value: ") + value);
        }
    }

    _values[argument.GetLongName()] = actual_value;
}

bool Arguments::HasArgumentValue(const string& name)
{
    const auto& argument = GetArgumentByName(name);
    if (argument.IsEmpty())
        throw exception((string("Unknown Argument: ") + name).c_str());

    return _values.find(argument.GetLongName()) != _values.end();
}

int Arguments::GetNextPosition() const
{
    return _last_position + 1;
}

void Arguments::AdvancePosition()
{
    ++_last_position;
}


//-----------------------------------------------------------------------------
// Public usage methods
Arguments::Arguments()
    : Arguments(ParserContext::Empty())
{
}

Arguments::Arguments(const ParserContext& parser_context)
    : _parser_context(parser_context)
{
    AddStandardArguments();
    AddFileOverrideArguments();
}

void Arguments::AddStandardArguments()
{
    AddSwitch(HelpShortName, HelpLongName, StringUtils::BoolToString(false), HelpDescription, false, INT_MAX - 2);
}
void Arguments::AddFileOverrideArguments()
{
    AddSwitch(UseDefaultArgumentsFileShortName, UseDefaultArgumentsFileLongName, StringUtils::BoolToString(true), GetUseDefaultArgumentsFileDesc(), false, INT_MAX - 1);
    AddSwitch(UseLocalArgumentsFileShortName  , UseLocalArgumentsFileLongName  , StringUtils::BoolToString(true), GetUseLocalArgumentsFileDesc()  , false, INT_MAX);
}

bool Arguments::IsEmpty() const
{
    return _arguments.empty();
}
void Arguments::Reset()
{
    _arguments.clear();
    _last_position = 0;
    _values.clear();
    _errors.clear();
}

list<Argument> Arguments::GetArguments() const
{
    const auto types = ArgumentTypeText.GetAllValues();

    return GetArgumentsByTypes(types);
}

list<Argument> Arguments::GetArgumentsByType(const ArgumentType ArgumentType) const
{
    list<Argument> filtered;

    for (auto iter = _arguments.begin(); iter != _arguments.end(); ++iter)
    {
        if (iter->second.GetArgumentType() == ArgumentType)
        {
            filtered.push_back(iter->second);
        }
    }

    return filtered;
}

list<Argument> Arguments::GetArgumentsByTypes(const list<ArgumentType>& ArgumentTypes) const
{
    list<Argument> filtered;

    for (auto iter = _arguments.begin(); iter != _arguments.end(); ++iter)
    {
        const auto found = std::find(std::begin(ArgumentTypes), std::end(ArgumentTypes), iter->second.GetArgumentType()) != std::end(ArgumentTypes);
        if (found)
        {
            filtered.push_back(iter->second);
        }
    }

    return filtered;
}

list<string> Arguments::GetErrors() const
{
    return _errors;
}

list<string> Arguments::GetNotes() const
{
    return _notes;
}

bool Arguments::IsValid() const
{
    return _errors.empty();
}

bool Arguments::IsHelp()
{
    const auto value = GetArgumentValue(HelpLongName);
    return ValueConverter::IsBool(value)
        ? ValueConverter::ToBool(value)
        : false;
}

bool Arguments::IsUsingDefaultArgumentsFile()
{
    const auto value = GetArgumentValue(UseDefaultArgumentsFileLongName);
    return ValueConverter::IsBool(value)
        ? ValueConverter::ToBool(value)
        : false;
}
