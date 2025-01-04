#pragma once

#include "pch.h"
#include "../DNX.App/Arguments.h"
#include "../DNX.App/Commands.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizeUseEqualsDefault

using namespace std;
using namespace DNX::App;

class AppArgumentsC1 final : public Arguments
{
public:
    AppArgumentsC1()
    {
        AddParameter(ValueType::STRING, 1, "item-name", "", "The name of the item", true);
        AddSwitch("v", "vflag", "false", "v flag", false, 0);
    }

    string GetItemName() { return GetArgumentValue("item-name"); }
    bool GetFlagV() { return GetSwitchValue("vflag"); }
};

class AppArgumentsC2 final : public Arguments
{
public:
    AppArgumentsC2()
    {
        AddSwitch("w", "wflag", "false", "w flag", false, 0);
    }

    bool GetFlagW() { return GetSwitchValue("wflag"); }
};

class AppArgumentsC3 final : public Arguments
{
public:
    AppArgumentsC3()
    {
        AddSwitch("x", "xflag", "false", "x flag", false, 0);
    }

    bool GetFlagX() { return GetSwitchValue("xflag"); }
};

class Commands1 final : public Commands
{
    AppArgumentsC1 _arguments_c1;
    AppArgumentsC2 _arguments_c2;
    AppArgumentsC3 _arguments_c3;

public:
    Commands1()
    {
        AddCommand(&_arguments_c1, "c1", "Command 1");
        AddCommand(&_arguments_c2, "c2", "Command 2");
        AddCommand(&_arguments_c3, "c3", "Command 3");
    }
};
