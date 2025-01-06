#pragma once
#include "stdafx.h"
#include "../DNX.Utils/EnumUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceEnumSize
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace DNX::Utils::EnumUtils;

namespace DNX::App
{
    //--------------------------------------------------------------------------
    // Enum: ValueType
    //--------------------------------------------------------------------------
    enum class ValueType : uint8_t
    {
        CHAR,
        STRING,
        BOOL,
        INT,
        DOUBLE,
        ENUM,
        DATE,
        DATETIME
    };

    //--------------------------------------------------------------------------
    // Class: ValueTypeTextResolver
    //--------------------------------------------------------------------------
    class ValueTypeTextResolver : public EnumTextResolver<ValueType>
    {
    public:
        ValueTypeTextResolver();
    };

    static ValueTypeTextResolver ValueTypeTextConverter;
}
