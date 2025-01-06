#pragma once

#include "pch.h"

using namespace std;

// ReSharper disable CppClangTidyModernizeUseEqualsDefault

class TestItem
{
public:
    string Text;

    TestItem() { Text = "empty"; }
    explicit TestItem(const string& text) { Text = text; }
};
