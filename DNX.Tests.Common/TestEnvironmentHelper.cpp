#include "stdafx.h"
#include "TestEnvironmentHelper.h"
#include "RunFolderPathManager.h"
#include "../DNX.Utils/DateUtils.h"
#include "../DNX.Utils/EnvironmentUtils.h"

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

// ReSharper disable CppInconsistentNaming

void TestEnvironmentHelper::Setup(const AppDetails& app_info)
{
    EnvironmentUtils::SetEnvironmentVariableValue("APP_VERSION", app_info.GetVersionDetails());
}

void TestEnvironmentHelper::Setup(const RunIdGenerator& run_id_generator)
{
    const auto start_datetime = run_id_generator.GetStartDateTime();

    EnvironmentUtils::SetEnvironmentVariableValue("DATE_CURRENTYEAR", DateUtils::FormatDate(&start_datetime, "%Y"));
}

void TestEnvironmentHelper::Setup(const RunFolderPathManager& run_folder_path_manager)
{
    EnvironmentUtils::SetEnvironmentVariableValue("RUN_FOLDERNAME", run_folder_path_manager.GetRunFolderPath());
}
