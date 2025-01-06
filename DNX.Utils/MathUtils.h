#pragma once

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::Utils
{
    //--------------------------------------------------------------------------
    // Class: MathUtils
    //--------------------------------------------------------------------------
    class MathUtils
    {
    public:
        static void ReseedRandomizer();
        static void ReseedRandomizer(int seed);
        static int GetRandomNumber(int max_value, bool inclusive = false);
        static int GetRandomNumber(int min_value, int max_value, bool inclusive = false);
    };
}
