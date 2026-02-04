#include "stdafx.h"

#define WIN32_LEAN_AND_MEAN
#define _HAS_STD_BYTE 0

#include "GuidGenerator.h"
#include "DateTime.h"
#include "StringUtils.h"
#include <filesystem>
#include <objbase.h>
#include <sstream>

#pragma comment(lib,"ole32.lib")

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppFunctionResultShouldBeUsed
// ReSharper disable CppClangTidyClangDiagnosticCoveredSwitchDefault
// ReSharper disable CppTooWideScopeInitStatement

using namespace DNX::Utils;
using namespace DNX::Utils::Crypto;

uint8_t GuidGenerator::m_DateTimeOffsetModifier = 0;
string GuidGenerator::m_lastDateTimeOffsetTimestamp;

string GuidGenerator::FormatAsGuid(const string& text)
{
    try
    {
        return text.substr(0, 8)
            + "-"
            + text.substr(8, 4)
            + "-"
            + text.substr(12, 4)
            + "-"
            + text.substr(16, 4)
            + "-"
            + text.substr(20, 12)
            ;
    }
    catch (const exception& ex)
    {
        return "";
    }
}

string GuidGenerator::GenerateGuid(const GuidGeneratorType generator_type)
{
    switch (generator_type)
    {
        case GuidGeneratorType::Default:
        case GuidGeneratorType::OS:
            return GenerateOSGuid();

        case GuidGeneratorType::DateTimeOffset:
            return GenerateDateTimeGuid();

        default:
            return GenerateOSGuid();
    }
}

// Source : https://stackoverflow.com/questions/1327157/whats-the-c-version-of-guid-newguid
string GuidGenerator::GenerateOSGuid()
{
    GUID guid;
    CoInitialize(nullptr);
    CoCreateGuid(&guid);

    std::ostringstream os;
    os << std::hex << std::setw(8) << std::setfill('0') << guid.Data1;
    os << '-';
    os << std::hex << std::setw(4) << std::setfill('0') << guid.Data2;
    os << '-';
    os << std::hex << std::setw(4) << std::setfill('0') << guid.Data3;
    os << '-';
    os << std::hex << std::setw(2) << std::setfill('0') << static_cast<short>(guid.Data4[0]);
    os << std::hex << std::setw(2) << std::setfill('0') << static_cast<short>(guid.Data4[1]);
    os << '-';
    os << std::hex << std::setw(2) << std::setfill('0') << static_cast<short>(guid.Data4[2]);
    os << std::hex << std::setw(2) << std::setfill('0') << static_cast<short>(guid.Data4[3]);
    os << std::hex << std::setw(2) << std::setfill('0') << static_cast<short>(guid.Data4[4]);
    os << std::hex << std::setw(2) << std::setfill('0') << static_cast<short>(guid.Data4[5]);
    os << std::hex << std::setw(2) << std::setfill('0') << static_cast<short>(guid.Data4[6]);
    os << std::hex << std::setw(2) << std::setfill('0') << static_cast<short>(guid.Data4[7]);

    CoUninitialize();

    return os.str();
}

string GuidGenerator::GenerateDateTimeGuid()
{
    const DateTime timestamp = DateTime::Now();

    const auto formatted_timestamp = timestamp.ToString(DateTime::Formats::Display);
    if (formatted_timestamp == m_lastDateTimeOffsetTimestamp)
    {
        m_DateTimeOffsetModifier++;
    }
    else
    {
        m_lastDateTimeOffsetTimestamp = formatted_timestamp;
        m_DateTimeOffsetModifier = 0;
    }

    auto text = timestamp.ToString("%y");
    text += timestamp.ToString("%m");
    text += timestamp.ToString("%d");
    text += timestamp.ToString("%H");
    text += timestamp.ToString("%M");
    text += StringUtils::Right(StringUtils::LPad(to_string(timestamp.GetSeconds()), 2, '0'), 2);
    text += StringUtils::Right(StringUtils::LPad(to_string(timestamp.GetMilliseconds() % 1000), 3, '0'), 3);
    text += StringUtils::Right(to_string(m_DateTimeOffsetModifier), 1);

    stringstream value;
    for (const auto ch : text)
    {
        value << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(ch));
    }

    return FormatAsGuid(value.str());
}
