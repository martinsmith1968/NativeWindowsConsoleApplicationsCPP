#include "stdafx.h"

#include "Guid.h"
#include "GuidGenerator.h"
#include "../StringUtils.h"
#include <filesystem>

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppFunctionResultShouldBeUsed

using namespace DNX::Utils::Crypto;

Guid::Guid(const array<unsigned char, 16> bytes)
{
    m_bytes = bytes;
}

Guid::Guid(const string& guid)
{
    ParseHex(guid, m_bytes);
}

string Guid::str() const
{
    return str(GuidFormatType::Default);
}

string Guid::str(const GuidFormatType format_type) const
{
    switch (format_type)
    {
        case GuidFormatType::Default:
        case GuidFormatType::Formatted:
        {
            const auto digits = str(GuidFormatType::Digits);

            return FormatBytes(GetBlock1Bytes())
                + "-"
                + FormatBytes(GetBlock2Bytes())
                + "-"
                + FormatBytes(GetBlock3Bytes())
                + "-"
                + FormatBytes(GetBlock4Bytes())
                + "-"
                + FormatBytes(GetBlock5Bytes())
                ;
        }

        case GuidFormatType::FormattedUpperCase:
            return StringUtils::ToUpper(str(GuidFormatType::Formatted));

        case GuidFormatType::Digits:
        {
            return FormatBytes(m_bytes);
        }

        case GuidFormatType::DigitsUpperCase:
            return StringUtils::ToUpper(str(GuidFormatType::Digits));

        default:
            return "";
    }
}

Guid Guid::NewGuid()
{
    return NewGuid(GuidGeneratorType::OS);
}

Guid Guid::NewGuid(const GuidGeneratorType generator_type)
{
    switch (generator_type)
    {
        case GuidGeneratorType::DateTimeOffset:
            return Guid(GuidGenerator::GenerateDateTimeGuid());

        case GuidGeneratorType::Default:
        case GuidGeneratorType::OS:
        default:  // NOLINT(clang-diagnostic-covered-switch-default)
        {
            return Guid(GuidGenerator::GenerateOSGuid());
        }
    }
}

Guid Guid::Empty()
{
    constexpr array<unsigned char, 16> bytes = { };
    Reset(bytes);

    return Guid(bytes);
}
