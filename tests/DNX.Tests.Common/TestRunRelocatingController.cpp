#include "stdafx.h"
#include "TestRunRelocatingController.h"
#include "TestHelper.h"
#include "../../libs/DNX.Utils/DirectoryUtils.h"
#include "../../libs/DNX.Utils/PathUtils.h"

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppPassValueParameterByConstReference
// ReSharper disable CppClangTidyPerformanceUnnecessaryValueParam

TestRunRelocatingController::TestRunRelocatingController(testing::UnitTest* unit_test_instance, AppDetails app_details, const string& executable_name, const string& slug_id)
    : TestRunController(unit_test_instance, app_details, executable_name, slug_id)
{
    m_run_id          = m_run_id_generator.GetRunId();
    m_run_folder_path = PathUtils::Combine(PathUtils::GetTempPath(), "test" + GetSlugId() + m_run_id);

    m_app_relocator.SetAppFolderPath(m_run_folder_path);
    m_relocated_executable_filepath = m_app_relocator.CopyFileToAppFolder(m_executable_filepath, true);
}

void TestRunRelocatingController::SetUp()
{
    TestRunController::SetUp();

    m_folder_history_manager.ChangeCurrentFolderTo(PathUtils::GetTempPath());

    TestHelper::SetEnvironment(m_run_id_generator);
    TestHelper::SetEnvironment(m_app_relocator);
}

void TestRunRelocatingController::TearDown()
{
    m_app_relocator.Cleanup();
    m_folder_history_manager.Cleanup();
}
