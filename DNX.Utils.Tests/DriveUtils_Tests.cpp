#include "pch.h"

#include "../DNX.Utils/DateUtils.h"
#include "../DNX.Utils/DirectoryUtils.h"
#include "../DNX.Utils/DriveUtils.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/PathUtils.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppTooWideScopeInitStatement

#define TEST_GROUP DriveUtils

TEST(TEST_GROUP, GetCurrentDriveReference_returns_as_expected)
{
    const auto& temp_path = PathUtils::GetTempPath();
    DirectoryUtils::SetCurrentDirectory(temp_path);

    // Act
    const auto result = DriveUtils::GetCurrentDriveReference();

    // Assert
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(PathUtils::GetDriveReference(temp_path), result);
}

TEST(TEST_GROUP, GetCurrentDriveId_returns_as_expected)
{
    const auto& temp_path = PathUtils::GetTempPath();
    DirectoryUtils::SetCurrentDirectory(temp_path);

    // Act
    const auto result = DriveUtils::GetCurrentDriveId();

    // Assert
    EXPECT_TRUE(result >= 1 && result <= 26);
    EXPECT_EQ(PathUtils::GetDriveId(temp_path), result);
}

TEST(TEST_GROUP, GetAllDriveReferences_returns_as_expected)
{
    // Act
    const auto result = DriveUtils::GetAllDriveReferences();
    cout << "Found " << result.size() << " drive references." << endl;

    // Assert
    EXPECT_FALSE(result.empty());

    for (const auto& drive_reference : result)
    {
        cout << "Found Drive Reference: " << drive_reference << endl;

        EXPECT_TRUE(PathUtils::IsDriveReference(drive_reference));
    }
}

TEST(TEST_GROUP, GetAllDrives_returns_as_expected)
{
    // Act
    const auto result = DriveUtils::GetAllDrives();
    cout << "Found " << result.size() << " drives." << endl;

    // Assert
    EXPECT_FALSE(result.empty());

    for (const auto& [drive_reference, drive_type] : result)
    {
        const DriveTypeTextResolver resolver;

        cout << "Found Drive Reference: " << drive_reference << ", type: " << resolver.GetText(drive_type) << " (" << static_cast<int>(drive_type) << ")" << endl;

        EXPECT_TRUE(PathUtils::IsDriveReference(drive_reference));
        EXPECT_GE(PathUtils::GetDriveId(drive_reference), 1);
        EXPECT_LE(PathUtils::GetDriveId(drive_reference), 26);
    }

    EXPECT_EQ(DriveUtils::GetAllDriveReferences().size(), result.size());
}

TEST(TEST_GROUP, GetDriveTypeFromReference_returns_as_expected)
{
    // Arrange
    const auto drive_references = DriveUtils::GetAllDriveReferences();
    set<DriveType> found_drive_types;

    for (const auto& drive_reference : drive_references)
    {
        // Act
        const auto drive_type = DriveUtils::GetDriveTypeFromReference(drive_reference);
        if (found_drive_types.find(drive_type) == found_drive_types.end())
        {
            const DriveTypeTextResolver resolver;

            found_drive_types.emplace(drive_type);

            cout << "Drive Reference: " << drive_reference << ", type: " << resolver.GetText(drive_type) << " (" << static_cast<int>(drive_type) << ")" << endl;

            // Assert
            EXPECT_TRUE(resolver.IsValid(drive_type));
            EXPECT_FALSE(resolver.GetText(drive_type).empty());
        }
    }
}

TEST(TEST_GROUP, SetCurrentDrive_by_drive_reference_functions_as_expected)
{
    const auto initial_directory = DirectoryUtils::GetCurrentDirectory();
    const auto initial_drive_reference = PathUtils::GetDriveReference(initial_directory);
    cout << "Initial CurrentDirectory: " << initial_directory << " (" << initial_drive_reference << ")" << endl;

    string new_drive_reference;

    try
    {
        const auto temp_path = PathUtils::GetTempPath();
        const auto temp_drive_reference = PathUtils::GetDriveReference(PathUtils::GetTempPath());

        const auto drive_references = DriveUtils::GetAllDriveReferences();

        DirectoryUtils::SetCurrentDirectory(temp_path);

        for (const auto& drive_reference : drive_references)
        {
            if (drive_reference == initial_drive_reference)
                continue;

            // Act
            const auto set_result = DriveUtils::SetCurrentDrive(drive_reference);
            if (!set_result)
                continue;

            new_drive_reference = DriveUtils::GetCurrentDriveReference();
            const auto new_directory = DirectoryUtils::GetCurrentDirectory();
            cout << "Set Current Drive to: " << drive_reference
                << ", result: " << (set_result ? "Success" : "Failure")
                << ", Current Drive: " << new_drive_reference << " (" << new_directory << ")" << endl;

            // Assert
            EXPECT_EQ(new_drive_reference, drive_reference);
        }
    }
    catch (const exception& ex)
    {
        cout << "ERROR: " << ex.what() << endl;
        FAIL();
    }

    if (!initial_drive_reference.empty())
        DriveUtils::SetCurrentDrive(initial_drive_reference);

    // Assert
    EXPECT_FALSE(new_drive_reference.empty());
}

TEST(TEST_GROUP, SetCurrentDrive_by_drive_id_functions_as_expected)
{
    const auto initial_directory = DirectoryUtils::GetCurrentDirectory();
    const auto initial_drive_reference = PathUtils::GetDriveReference(initial_directory);
    cout << "Initial CurrentDirectory: " << initial_directory << " (" << initial_drive_reference << ")" << endl;

    int new_drive_id;

    try
    {
        const auto temp_path = PathUtils::GetTempPath();
        const auto temp_drive_reference = PathUtils::GetDriveReference(PathUtils::GetTempPath());

        const auto drive_references = DriveUtils::GetAllDriveReferences();

        DirectoryUtils::SetCurrentDirectory(temp_path);

        for (const auto& drive_reference : drive_references)
        {
            if (drive_reference == initial_drive_reference)
                continue;

            const auto drive_id = PathUtils::GetDriveId(drive_reference);

            // Act
            const auto set_result = DriveUtils::SetCurrentDrive(drive_id);
            if (!set_result)
                continue;

            new_drive_id = DriveUtils::GetCurrentDriveId();
            const auto new_directory = DirectoryUtils::GetCurrentDirectory();
            cout << "Set Current Drive to: " << drive_reference
                << ", result: " << (set_result ? "Success" : "Failure")
                << ", Current Drive: " << new_drive_id << " (" << new_directory << ")" << endl;

            // Assert
            EXPECT_EQ(new_drive_id, drive_id);
        }
    }
    catch (const exception& ex)
    {
        cout << "ERROR: " << ex.what() << endl;
        FAIL();
    }

    if (!initial_drive_reference.empty())
        DriveUtils::SetCurrentDrive(initial_drive_reference);

    // Assert
    EXPECT_GT(new_drive_id, 0);
}
