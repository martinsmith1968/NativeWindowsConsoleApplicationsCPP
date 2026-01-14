#pragma once

#include "pch.h"

// ReSharper disable CppClangTidyModernizeUseEqualsDefault
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppInconsistentNaming

using namespace std;

class TestItem
{
public:
    string Text;

    TestItem() { Text = "empty"; }
    explicit TestItem(const string& text) { Text = text; }
};
