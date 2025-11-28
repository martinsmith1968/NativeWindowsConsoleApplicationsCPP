#include "pch.h"

#include <cstdlib>
#include "../DNX.Tests.Common/TestHelper.h"
#include "../DNX.Utils/EnvironmentUtils.h"
#include "../DNX.Utils/PathUtils.h"

#define VER_PRODUCTNAME_STR         "Stopwatch.Tests"
#define VER_FILE_DESCRIPTION_STR    "Stopwatch Google Tests"
#include <filesystem>

#include "../Common/AppInfo.h"
#include "../DNX.Utils/DateUtils.h"
#include "../DNX.Utils/FileUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable StringLiteralTypo

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;
using namespace filesystem;

#define TEST_GROUP Stopwatch

//------------------------------------------------------------------------------

class TestConfig
{
public:
    static string GetExecutableName()
    {
        return "Stopwatch.exe";
    }

    static string GetExpectedOutputFileName()
    {
        return PathUtils::Combine("ExpectedOutput", string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".txt");
    }
};

//------------------------------------------------------------------------------

class TEST_GROUP : public ::testing::Test
{
protected:
    AppInfo _appInfo;
    string _run_id;
    string _defaultTargetExecutableFileName;
    string _movedTargetExecutableFileName;
    string _tempRunFolderName;

    void SetUp() override
    {
        const auto now = DateUtils::ToCalendarDateTime(DateUtils::GetNow());

        //srand(static_cast<unsigned>(time(nullptr)));
        //_run_id = to_string(rand());
        _run_id = DateUtils::FormatDate(&now, "%Y%m%d%H%M%S");

        EnvironmentUtils::SetEnvironmentVariableValue("APP_VERSION", _appInfo.GetVersionDetails());
        EnvironmentUtils::SetEnvironmentVariableValue("DATE_CURRENTYEAR", DateUtils::FormatDate(&now, "%Y"));

        const auto executableFileName = TestConfig::GetExecutableName();

        _defaultTargetExecutableFileName = TestHelper::FindExecutableFileName(executableFileName);

        _tempRunFolderName = PathUtils::Combine(PathUtils::GetTempPath(), "test" + _run_id);
        PathUtils::CreateDirectory(_tempRunFolderName);
        EnvironmentUtils::SetEnvironmentVariableValue("RUN_FOLDERNAME", _tempRunFolderName);

        _movedTargetExecutableFileName = PathUtils::Combine(PathUtils::GetTempPath(), FileUtils::GetFileNameAndExtension(_defaultTargetExecutableFileName));
        copy_file(_defaultTargetExecutableFileName.c_str(), _movedTargetExecutableFileName.c_str(), copy_options::overwrite_existing);
    }

    void TearDown() override
    {
        if (!_tempRunFolderName.empty() && PathUtils::DirectoryExists(_tempRunFolderName))
        {
            PathUtils::DeleteDirectory(_tempRunFolderName, true, true);
        }
    }
};

TEST_F(TEST_GROUP, Execute_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();
    chdir(_tempRunFolderName.c_str());
    EXPECT_EQ(TestHelper::ExecuteAndCaptureOutput(_movedTargetExecutableFileName, "-?"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}

TEST_F(TEST_GROUP, Execute_with_help_request_long_produces_command_list)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();
    chdir(_tempRunFolderName.c_str());
    EXPECT_EQ(TestHelper::ExecuteAndCaptureOutput(_movedTargetExecutableFileName, "--help"), TestHelper::GetExpectedOutput(expectedResultsFileName));
}
