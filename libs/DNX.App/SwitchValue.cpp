#include "stdafx.h"
#include "SwitchValue.h"
#include "../DNX.Utils/StringUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizeReturnBracedInitList
// ReSharper disable CppTooWideScopeInitStatement

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;

string SwitchValue::GetName() const
{
    return _name;
}
bool SwitchValue::GetValue() const
{
    return _value;
}

SwitchValue::SwitchValue(const string& name, const bool value)
{
    _name  = name;
    _value = value;
}

SwitchValue SwitchValue::ParseDetails(const string& optionName)
{
    auto realName = optionName;
    auto value = true;

    const auto switchValue = StringUtils::Right(optionName, 1);
    if (switchValue == "-")
    {
        value = false;
        realName = StringUtils::Left(optionName, optionName.length() - 1);
    }
    else if (switchValue == "+")
    {
        value = true;
        realName = StringUtils::Left(optionName, optionName.length() - 1);
    }

    return SwitchValue(realName, value);
}
