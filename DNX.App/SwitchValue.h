#pragma once

#include "stdafx.h"
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::App
{
    //--------------------------------------------------------------------------
    // Class: SwitchValue
    //--------------------------------------------------------------------------
    class SwitchValue {
        string _name;
        bool _value;

    public:
        [[nodiscard]] string GetName() const;
        [[nodiscard]] bool GetValue() const;

        SwitchValue(const string& name, bool value);

        static SwitchValue ParseDetails(const string& optionName);
    };
}
