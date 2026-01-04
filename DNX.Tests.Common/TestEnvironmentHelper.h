#pragma once

#include "stdafx.h"
#include "RunFolderPathManager.h"
#include "RunIdGenerator.h"
#include "../DNX.App/AppDetails.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;
using namespace DNX::App;

namespace DNX::Tests::Common
{
    class TestEnvironmentHelper
    {

    public:
        static void Setup(const AppDetails& app_info);
        static void Setup(const RunIdGenerator& run_id_generator);
        static void Setup(const RunFolderPathManager& run_folder_path_manager);
    };
}
