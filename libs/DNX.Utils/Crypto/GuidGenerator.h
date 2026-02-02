#pragma once

#include "../DateTime.h"
#include <string>

// ReSharper disable CppInconsistentNaming

using namespace std;

namespace DNX::Utils::Crypto
{
    enum class GuidGeneratorType : uint8_t
    {
        Default,
        OS,
        DateTimeOffset,
    };

    class GuidGenerator
    {
        static uint8_t m_DateTimeOffsetModifier;
        static string m_lastDateTimeOffsetTimestamp;

        static string FormatAsGuid(const string& text);

    public:
        static string GenerateGuid(GuidGeneratorType generator_type);

        [[nodiscard]] static string GenerateOSGuid();
        [[nodiscard]] static string GenerateDateTimeGuid();
    };
}
