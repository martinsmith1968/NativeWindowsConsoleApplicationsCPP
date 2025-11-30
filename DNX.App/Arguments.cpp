#include "stdafx.h"
#include "Argument.h"
#include "Arguments.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/ListUtils.h"
#include "../DNX.Utils/PathUtils.h"
#include "../DNX.Utils/StringUtils.h"
#include <complex>

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
string Arguments::GetUseDefaultArgumentsFileDesc() const
{
    return "Use Default Arguments File (" + PathUtils::GetFileNameAndExtension(_parser_context.GetDefaultArgumentsFileName()) + ")";
}
string Arguments::GetUseLocalArgumentsFileDesc() const
{
    // TODO: This should call GetLocalArgumentsFileName() from ParserContext, bu this causes the tests to hang
    return "Use Local Arguments File (" + PathUtils::GetFileNameAndExtension(_parser_context.GetDefaultArgumentsFileName()) + ")";
}

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
    const list<string>& valueList,
    const bool allowMultiple
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

    const auto argument = Argument(argumentType, valueType, realPosition, realShortName, longName, description, defaultValue, required, valueList, allowMultiple);

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
    const list<string>& valueList,
    const bool allowMultiple
)
{
    AddArgumentComplete(argumentType, valueType, shortName, longName, defaultValue, description, required, position, valueList, allowMultiple);
}

void Arguments::AddParameter(
    const ValueType valueType,
    const int position,
    const string& longName,
    const string& defaultValue,
    const string& description,
    const bool required,
    const list<string>& valueList,
    const bool allowMultiple
)
{
    int realPosition = position;
    if (position <= 0)
    {
        const auto types = { ArgumentType::PARAMETER };
        realPosition = static_cast<int>(GetArgumentsByTypes(types).size() + 1);
    }

    AddArgument(ArgumentType::PARAMETER, valueType, "", longName, defaultValue, description, required, realPosition, valueList, allowMultiple);
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

    AddArgument(ArgumentType::OPTION, valueType, shortName, longName, defaultValue, description, required, realPosition, valueList, false);
}

void Arguments::AddSwitch(
    const string& shortName,
    const string& longName,
    const bool defaultValue,
    const string& description,
    const bool required,
    const int position
)
{
    int realPosition = position;
    if (position <= 0)
    {
        const auto types = { ArgumentType::OPTION, ArgumentType::SWITCH };
        realPosition = static_cast<int>(GetArgumentsByTypes(types).size() + 1);
    }

    AddArgument(ArgumentType::SWITCH, ValueType::BOOL, shortName, longName, StringUtils::BoolToString(defaultValue), description, required, realPosition);
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
    Argument* multipleParameter = nullptr;
    for (auto iter = _arguments.begin(); iter != _arguments.end(); ++iter)
    {
        if (iter->second.GetArgumentType() == ArgumentType::PARAMETER)
        {
            if (iter->second.GetPosition() == position)
            {
                return iter->second;
            }
            if (iter->second.GetAllowMultiple())
                multipleParameter = &iter->second;
        }
    }

    if (multipleParameter)
        return *multipleParameter;

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

list<string> Arguments::GetArgumentValues(const string& name)
{
    const auto& argument = GetArgumentByName(name);
    if (argument.IsEmpty())
        throw exception((string("Unknown Argument: ") + name).c_str());

    const auto found = _multipleValues.find(argument.GetLongName());
    if (found != _multipleValues.end())
        return found->second;

    return {};
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

    if (argument.GetAllowMultiple())
    {
        if (_multipleValues.find(argument.GetLongName()) == _multipleValues.end())
            _multipleValues[argument.GetLongName()] = list<string>();
        _multipleValues[argument.GetLongName()].emplace_back(actual_value);
    }
    else
        _values[argument.GetLongName()] = actual_value;
}

bool Arguments::HasArgumentValue(const string& name)
{
    const auto& argument = GetArgumentByName(name);
    if (argument.IsEmpty())
        throw exception((string("Unknown Argument: ") + name).c_str());

    return argument.GetAllowMultiple()
        ? _multipleValues.find(argument.GetLongName()) != _multipleValues.end()
            && !_multipleValues[argument.GetLongName()].empty()
        : _values.find(argument.GetLongName()) != _values.end();
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

Arguments::Arguments(const Arguments& other)
    : _last_position(other._last_position)
    , _arguments(other._arguments)
    , _values(other._values)
    , _multipleValues(other._multipleValues)
    , _notes(other._notes)
    , _errors(other._errors)
    , _parser_context(other._parser_context)
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
    AddSwitch(VersionShortName, VersionLongName, false, VersionDescription, false, INT_MAX - 3);
    AddSwitch(HelpShortName, HelpLongName, false, HelpDescription, false, INT_MAX - 2);
}
void Arguments::AddFileOverrideArguments()
{
    AddSwitch(UseDefaultArgumentsFileShortName, UseDefaultArgumentsFileLongName, true, GetUseDefaultArgumentsFileDesc(), false, INT_MAX - 1);
    AddSwitch(UseLocalArgumentsFileShortName  , UseLocalArgumentsFileLongName  , true, GetUseLocalArgumentsFileDesc()  , false, INT_MAX);
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
    _multipleValues.clear();
    _errors.clear();
}

void Arguments::Verify()
{
    auto hasMultipleParameter = false;
    auto multipleParameterIndex = 0;

    auto index = 0;
    auto parameter_index = 0;
    for (const auto& [argument_name, argument] : _arguments)
    {
        ++index;
        if (argument.GetArgumentType() == ArgumentType::PARAMETER)
        {
            if (argument.GetAllowMultiple())
            {
                ++parameter_index;
                multipleParameterIndex = parameter_index;
                if (hasMultipleParameter)
                    _errors.push_back(argument_name + " cannot be defined to allow multiple arguments");
                else
                    hasMultipleParameter = true;
            }
        }
    }

    if (hasMultipleParameter)
    {
        if (multipleParameterIndex != parameter_index)
            _errors.emplace_back("The parameter supporting multiple values must be the last parameter");
    }
}

Arguments& Arguments::Empty()
{
    return _empty_arguments;
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

bool Arguments::IsVersion()
{
    const auto value = GetArgumentValue(VersionLongName);
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
