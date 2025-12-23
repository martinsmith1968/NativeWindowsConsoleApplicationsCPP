#include "pch.h"

#define VER_PRODUCTNAME_STR         "Stopwatch.Tests"
#define VER_FILE_DESCRIPTION_STR    "Stopwatch Google Tests"

#include "../Common/AppInfo.h"
#include "../DNX.Tests.Common/TestHelper.h"
#include "../DNX.Utils/DirectoryUtils.h"
#include "../DNX.Utils/EnvironmentUtils.h"
#include "../DNX.Utils/PathUtils.h"
#include "../DNX.Utils/DateUtils.h"
#include <direct.h>
#include <filesystem>
#include <string>

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
        DirectoryUtils::Create(_tempRunFolderName);
        EnvironmentUtils::SetEnvironmentVariableValue("RUN_FOLDERNAME", _tempRunFolderName);

        _movedTargetExecutableFileName = PathUtils::Combine(PathUtils::GetTempPath(), PathUtils::GetFileNameAndExtension(_defaultTargetExecutableFileName));
        copy_file(_defaultTargetExecutableFileName.c_str(), _movedTargetExecutableFileName.c_str(), copy_options::overwrite_existing);
    }

    void TearDown() override
    {
        if (!_tempRunFolderName.empty() && DirectoryUtils::Exists(_tempRunFolderName))
        {
            DirectoryUtils::Delete(_tempRunFolderName, true, true);
        }
    }
};

TEST_F(TEST_GROUP, Execute_with_help_request_produces_command_list)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();
    _chdir(_tempRunFolderName.c_str());
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(_movedTargetExecutableFileName, "-?"));
    TestHelper::WriteMajorSeparator(100);
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(_movedTargetExecutableFileName, "--help"));
}

TEST_F(TEST_GROUP, Execute_command_cancel_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();
    _chdir(_tempRunFolderName.c_str());
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(_movedTargetExecutableFileName, "cancel|-?"));
}

TEST_F(TEST_GROUP, Execute_command_elapsed_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();
    _chdir(_tempRunFolderName.c_str());
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(_movedTargetExecutableFileName, "elapsed|-?"));
}

TEST_F(TEST_GROUP, Execute_command_list_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();
    _chdir(_tempRunFolderName.c_str());
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(_movedTargetExecutableFileName, "list|-?"));
}

TEST_F(TEST_GROUP, Execute_command_pause_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();
    _chdir(_tempRunFolderName.c_str());
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(_movedTargetExecutableFileName, "pause|-?"));
}

TEST_F(TEST_GROUP, Execute_command_resume_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();
    _chdir(_tempRunFolderName.c_str());
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(_movedTargetExecutableFileName, "resume|-?"));
}

TEST_F(TEST_GROUP, Execute_command_start_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();
    _chdir(_tempRunFolderName.c_str());
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(_movedTargetExecutableFileName, "start|-?"));
}

TEST_F(TEST_GROUP, Execute_command_stop_with_help_request_short_produces_command_list)
{
    const auto expectedResultsFileName = TestConfig::GetExpectedOutputFileName();
    _chdir(_tempRunFolderName.c_str());
    EXPECT_EQ(TestHelper::GetExpectedOutput(expectedResultsFileName), TestHelper::ExecuteAndCaptureOutput(_movedTargetExecutableFileName, "stop|-?"));
}
