#include "stdafx.h"
#include "Argument.h"

// ReSharper disable CppInconsistentNaming

using namespace std;
using namespace DNX::App;

Argument Argument::_empty_argument = Argument();

bool Argument::IsEmpty() const
{
    return _position == 0;
}
ArgumentType Argument::GetArgumentType() const
{
    return _argumentType;
}
ValueType Argument::GetValueType() const
{
    return _valueType;
}
string Argument::GetShortName() const
{
    return _shortName;
}
string Argument::GetLongName() const
{
    return _longName;
}
string Argument::GetDescription() const
{
    return _description;
}
string Argument::GetDefaultValue() const
{
    return _defaultValue;
}
bool Argument::GetRequired() const
{
    return _required;
}
uint8_t Argument::GetPosition() const
{
    return _position;
}
list<string> Argument::GetValueList() const
{
    return _valueList;
}
bool Argument::GetAllowMultiple() const
{
    return _allowMultiple;
}

bool Argument::HasLongName() const
{
    return !_longName.empty();
}

string Argument::GetNameDescription() const
{
    string description;

    description += ArgumentTypeTextHelper.GetText(_argumentType);

    if (!description.empty())
    {
        description += ": ";
    }

    description += _longName;

    return description;
}

Argument::Argument() :
    _argumentType(ArgumentType::PARAMETER),
    _valueType(ValueType::STRING),
    _required(false),
    _position(0),
    _allowMultiple(false)
{
}

Argument::Argument(
    const ArgumentType argumentType,
    const ValueType valueType,
    const uint8_t position,
    const string& shortName,
    const string& longName,
    const string& description,
    const string& defaultValue,
    const bool required,
    const list<string>& valueList,
    const bool allowMultiple
)
{
    _argumentType  = argumentType;
    _valueType     = valueType;
    _shortName     = shortName;
    _longName      = longName;
    _description   = description;
    _defaultValue  = defaultValue;
    _required      = required;
    _position      = position;
    _allowMultiple = allowMultiple;

    if (!valueList.empty())
    {
        _valueList = valueList;
    }
}

bool Argument::CompareByPosition(const Argument& first, const Argument& second)
{
    return first.GetPosition() < second.GetPosition();
}

bool Argument::AreOptionTypesEqual(ArgumentType first, ArgumentType second)
{
    if (first == second)
        return true;

    if (first == ArgumentType::SWITCH && second == ArgumentType::OPTION)
        return true;

    if (first == ArgumentType::OPTION && second == ArgumentType::SWITCH)
        return true;

    return false;
}

bool Argument::CompareByTypeAndPosition(const Argument& first, const Argument& second)
{
    if (!AreOptionTypesEqual(first.GetArgumentType(), second.GetArgumentType()))
    {
        return first.GetArgumentType() < second.GetArgumentType();
    }

    return CompareByPosition(first, second);
}
