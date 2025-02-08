#pragma once

#include "BuildInfo.h"
#include "Version.h"
#include "Macros.h"

#include "../DNX.App/AppDetails.h"

using namespace DNX::App;

// ReSharper disable IdentifierTypo
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizeUseEqualsDefault

#define COMPANY_DATA_FOLDER_NAME    "DNXSolutions"
#define VER_COMPANY_NAME            "Martin Smith"
#define VER_COPYRIGHT_STR           "Copyright © 2018-" TO_STR(COPYRIGHT_YEAR) " " VER_COMPANY_NAME

#define VER_FILE_VERSION            VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD, VERSION_MODIFIER
#define VER_FILE_VERSION_STR        TO_STR(VERSION_MAJOR)        \
                                    "." TO_STR(VERSION_MINOR)    \
                                    "." TO_STR(VERSION_REVISION) \
                                    "." TO_STR(VERSION_BUILD)    \
                                    VERSION_MODIFIER

#define VER_PRODUCT_VERSION         VERSION_MAJOR, VERSION_MINOR
#define VER_PRODUCT_VERSION_STR     TO_STR(VERSION_MAJOR)        \
                                    "." TO_STR(VERSION_MINOR)
#define VER_ORIGINAL_FILENAME_STR   VER_PRODUCTNAME_STR ".exe"
#define VER_INTERNAL_NAME_STR       VER_ORIGINAL_FILENAME_STR

#define VER_COMMENTS                "Build Timestamp : " BUILD_DATETIME

#ifdef _DEBUG
#define VER_VER_DEBUG               VS_FF_DEBUG
#else
#define VER_VER_DEBUG               0
#endif

#define VER_FILEOS                  VOS_NT_WINDOWS32
#define VER_FILEFLAGS               VER_VER_DEBUG
#define VER_FILETYPE                VFT_APP


//------------------------------------------------------------------------------
// AppInfo
class AppInfo final : public AppDetails
{
public:
    AppInfo()
    {
        Name        = VER_PRODUCTNAME_STR;
        Description = VER_FILE_DESCRIPTION_STR;
        Version     = VER_FILE_VERSION_STR;
        Copyright   = VER_COPYRIGHT_STR;
    }
};
