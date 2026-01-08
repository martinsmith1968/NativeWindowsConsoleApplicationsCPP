#pragma once

#include "stdafx.h"
#include "../DNX.App/AppDetails.h"
#include "gtest/gtest.h"
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticPadded
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppPolymorphicClassWithNonVirtualPublicDestructor
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions

using namespace std;
using namespace DNX::App;

namespace DNX::Tests::Common
{
    class TestRunController
    {
    protected:
        testing::UnitTest* m_unit_test_instance;
        AppDetails m_app_details;
        string m_executable_name;
        string m_slug_id;
        string m_executable_filepath;

    public:
        explicit TestRunController(testing::UnitTest* unit_test_instance, AppDetails app_details, const string& executable_name, const string& slug_id = "");

        string& GetExecutableName() { return m_executable_name; }
        string& GetSlugId() { return m_slug_id; }
        string& GetExecutableFilePath() { return m_executable_filepath; }

        [[nodiscard]] string GetExpectedOutputFileName() const;

        virtual void SetUp();
        virtual void TearDown();
    };
}
