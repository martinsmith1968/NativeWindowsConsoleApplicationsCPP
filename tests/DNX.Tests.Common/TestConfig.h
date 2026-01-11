#pragma once

#include "gtest/gtest.h"
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticPadded
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::Tests::Common
{
    class TestConfig
    {
        testing::UnitTest* m_unit_test_instance;
        string m_executable_name;
        string m_slug_id;

    public:
        explicit TestConfig(testing::UnitTest* unit_test_instance, const string& executable_name, const string& slug_id = "");

        string& GetExecutableName() { return m_executable_name; }
        string& GetSlugId() { return m_slug_id; }

        [[nodiscard]] string GetExpectedOutputFileName() const;
    };
}
