#include "stdafx.h"
#include "CryptoUtils.h"

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming

using namespace DNX::Utils;

int CryptoUtils::GetRandomNumber()
{
    return rand();
}

int CryptoUtils::GetRandomNumber(const int min, const int max)
{
    return rand() % (max - min) + min;
}
