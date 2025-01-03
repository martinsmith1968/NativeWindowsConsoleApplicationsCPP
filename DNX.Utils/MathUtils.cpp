#include "stdafx.h"
#include "MathUtils.h"
#include <cstdlib>
#include <ctime>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCertMsc51Cpp
// ReSharper disable CppClangTidyClangDiagnosticShorten64To32

using namespace std;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: Math
//--------------------------------------------------------------------------

void MathUtils::ReseedRandomizer()
{
    srand(time(nullptr));
}

int MathUtils::GetRandomNumber(const int max_value, const bool inclusive)
{
    return GetRandomNumber(0, max_value, inclusive);
}

int MathUtils::GetRandomNumber(const int min_value, const int max_value,const bool inclusive)
{
    const auto adjustment = inclusive ? 1 : 0;

    const auto result = rand() % (max_value - min_value + adjustment) + min_value;

    return result;
}
