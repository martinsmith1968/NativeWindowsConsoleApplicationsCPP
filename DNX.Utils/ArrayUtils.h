#pragma once

#pragma warning( disable : 6384 )

#include "stdafx.h"

// ReSharper disable CppInconsistentNaming

namespace DNX::Utils
{
    //--------------------------------------------------------------------------
    // Class: ArrayUtils
    //--------------------------------------------------------------------------
    class ArrayUtils
    {
    public:
        template<typename T>
        static size_t GetSize(T values[])
        {
            return sizeof(values) / sizeof(T);
        }
    };
}
