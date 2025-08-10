#pragma once

#include <cstdlib>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::Utils
{
    class CryptoUtils
    {
    public:
        static int GetRandomNumber();
        static int GetRandomNumber(int min, int max);
    };
}
