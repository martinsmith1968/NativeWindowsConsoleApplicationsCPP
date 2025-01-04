#pragma once

#include "../DNX.App/Arguments.h"
#include "../DNX.App/Commands.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

class TestHelper
{
public:
    static void ShowErrors(const Arguments& arguments);
    static void ShowErrors(const Commands& commands);
    static void ShowErrors(const Command& command);
};
