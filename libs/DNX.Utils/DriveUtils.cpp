#include "stdafx.h"
#include "DriveUtils.h"
#include "PathUtils.h"
#include <Windows.h>
#include <direct.h>
#include <filesystem>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppTooWideScopeInitStatement

using namespace std;
using namespace std::filesystem;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: DriveUtils
//--------------------------------------------------------------------------

DriveType DriveUtils::ConvertToDriveType(const int drive_type_id)
{
/*
From Windows.h

#define DRIVE_UNKNOWN     0
#define DRIVE_NO_ROOT_DIR 1
#define DRIVE_REMOVABLE   2
#define DRIVE_FIXED       3
#define DRIVE_REMOTE      4
#define DRIVE_CDROM       5
#define DRIVE_RAMDISK     6
*/
    switch (drive_type_id)
    {
        case DRIVE_NO_ROOT_DIR:
            return DriveType::NoRootDir;
        case DRIVE_REMOVABLE:
            return DriveType::Removable;
        case DRIVE_FIXED:
            return DriveType::Fixed;
        case DRIVE_REMOTE:
            return DriveType::Remote;
        case DRIVE_CDROM:
            return DriveType::CDROM;
        case DRIVE_RAMDISK:
            return DriveType::RAMDISK;
        default:
            return DriveType::Unknown;
    }
}

string DriveUtils::GetCurrentDriveReference()
{
    return PathUtils::GetDriveReference(GetCurrentDriveId());
}

int DriveUtils::GetCurrentDriveId()
{
    const auto drive_id = _getdrive();

    return drive_id;
}

list<string> DriveUtils::GetAllDriveReferences()
{
    constexpr int MAX_BUFFER_SIZE = 500;
    char buffer[MAX_BUFFER_SIZE];

    list<string> drives;

    if (GetLogicalDriveStringsA(MAX_BUFFER_SIZE, buffer))
    {
        for (int i = 0; i < 100; i += 4)
        {
            if (buffer[i] == static_cast<char>(0))
                break;

            auto drive_reference = std::string{ buffer[i], buffer[i + 1] };
            drives.emplace_back(drive_reference);
        }
    }

    return drives;
}

map<string, DriveType> DriveUtils::GetAllDrives()
{
    map<string, DriveType> drives;

    const auto drive_references = GetAllDriveReferences();

    for (const string& drive_reference : drive_references)
    {
        const auto drive_type = GetDriveTypeFromReference(drive_reference);

        drives.emplace(drive_reference, drive_type);
    }

    return drives;
}

DriveType DriveUtils::GetDriveTypeFromReference(const string& drive_reference)
{
    if (!PathUtils::IsDriveReference(drive_reference))
        return DriveType::Unknown;

    const auto drive_type_id = GetDriveTypeA(drive_reference.c_str());

    const auto drive_type = ConvertToDriveType(drive_type_id);

    return drive_type;
}

bool DriveUtils::SetCurrentDrive(const string& drive_reference)
{
    const auto drive_id = PathUtils::GetDriveId(drive_reference);

    return SetCurrentDrive(drive_id);
}

bool DriveUtils::SetCurrentDrive(const int drive_id)
{
    if (drive_id < 1 || drive_id > 26)
        return false;

    if (_chdrive(drive_id) != 0)
        return false;

    return true;
}
