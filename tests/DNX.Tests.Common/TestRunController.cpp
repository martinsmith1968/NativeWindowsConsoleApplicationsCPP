#include "stdafx.h"
#include "TestRunController.h"
#include "TestHelper.h"
#include "../DNX.Utils/PathUtils.h"

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppPassValueParameterByConstReference
// ReSharper disable CppClangTidyPerformanceUnnecessaryValueParam

TestRunController::TestRunController(testing::UnitTest* unit_test_instance, AppDetails app_details, const string& executable_name, const string& slug_id)
{
    m_unit_test_instance  = unit_test_instance;
    m_app_details         = app_details;
    m_executable_name     = executable_name;
    m_slug_id             = slug_id;
    m_executable_filepath = TestHelper::FindExecutableFilePath(m_executable_name, true);
}

string TestRunController::GetExpectedOutputFileName() const
{
    return PathUtils::Combine("ExpectedOutput", string(m_unit_test_instance->current_test_info()->name()) + ".txt");
}

void TestRunController::SetUp()
{
    TestHelper::SetEnvironment(m_app_details);
}

void TestRunController::TearDown()
{
}
