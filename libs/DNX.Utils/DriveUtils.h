#pragma once

#include "stdafx.h"

#include "EnumUtils.h"

#include <list>
#include <map>
#include <string>

// ReSharper disable CppInconsistentNaming

using namespace std;

namespace DNX::Utils
{
    enum class DriveType : uint8_t
    {
        Unknown   = 0,
        NoRootDir = 1,
        Removable = 2,
        Fixed     = 3,
        Remote    = 4,
        CDROM     = 5,
        RAMDISK   = 5
    };

    class DriveTypeTextResolver : public EnumUtils::EnumTextResolver<DriveType>
    {
    public:
        DriveTypeTextResolver()
        {
            SetText(DriveType::Unknown, "Unknown");
            SetText(DriveType::NoRootDir, "No Root Dir");
            SetText(DriveType::Removable, "Removable");
            SetText(DriveType::Fixed, "Fixed");
            SetText(DriveType::Remote, "Remote");
            SetText(DriveType::CDROM, "CDROM");
            SetText(DriveType::RAMDISK, "RAM Disk");
        }
    };

    //--------------------------------------------------------------------------
    // Class: DriveUtils
    //--------------------------------------------------------------------------
    class DriveUtils
    {
        static DriveType ConvertToDriveType(int drive_type_id);

    public:
        static string GetCurrentDriveReference();
        static int GetCurrentDriveId();

        static list<string> GetAllDriveReferences();
        static map<string, DriveType> GetAllDrives();

        static DriveType GetDriveTypeFromReference(const string& drive_reference);

        static bool SetCurrentDrive(const string& drive_reference);
        static bool SetCurrentDrive(int drive_id);
    };
}
