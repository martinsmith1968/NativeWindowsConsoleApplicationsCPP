#pragma once

#include "stdafx.h"
#include "ArgumentType.h"
#include "ValueType.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;
using namespace DNX::App;

namespace DNX::App
{
    //--------------------------------------------------------------------------
    // Class: Argument
    //--------------------------------------------------------------------------
    class Argument
    {
        ArgumentType _argumentType;
        ValueType _valueType;
        string _shortName;
        string _longName;
        string _description;
        string _defaultValue;
        bool _required;
        uint8_t _position;
        list<string> _valueList{};
        bool _allowMultiple;

        static Argument _empty_argument;

    protected:
        ArgumentTypeTextResolver ArgumentTypeTextHelper;

    public:
        [[nodiscard]] bool IsEmpty() const;

        [[nodiscard]] ArgumentType GetArgumentType() const;
        [[nodiscard]] ValueType GetValueType() const;
        [[nodiscard]] string GetShortName() const;
        [[nodiscard]] string GetLongName() const;
        [[nodiscard]] string GetDescription() const;
        [[nodiscard]] string GetDefaultValue() const;
        [[nodiscard]] bool GetRequired() const;
        [[nodiscard]] uint8_t GetPosition() const;
        [[nodiscard]] list<string> GetValueList() const;
        [[nodiscard]] bool GetAllowMultiple() const;

        [[nodiscard]] bool HasLongName() const;
        [[nodiscard]] string GetNameDescription() const;

        Argument();

        Argument(
            ArgumentType argumentType,
            ValueType valueType,
            uint8_t position,
            const string& shortName,
            const string& longName = "",
            const string& description = "",
            const string& defaultValue = "",
            bool required = false,
            const list<string>& valueList = list<string>(),
            bool allowMultiple = false
        );

        static Argument& Empty() { return _empty_argument; }

        static bool CompareByPosition(const Argument& first, const Argument& second);
        static bool AreOptionTypesEqual(ArgumentType first, ArgumentType second);
        static bool CompareByTypeAndPosition(const Argument& first, const Argument& second);
    };
}
