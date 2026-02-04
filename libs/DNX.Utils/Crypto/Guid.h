#pragma once

#include "GuidGenerator.h"
#include "../StringUtils.h"
#include <array>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers

using namespace std;

namespace DNX::Utils::Crypto
{

    enum class GuidFormatType : uint8_t
    {
        Default,
        Formatted,
        Digits,
        FormattedUpperCase,
        DigitsUpperCase,
    };

    class Guid
    {
        //----------------------------------------------------------------------

        array<unsigned char, 16> m_bytes = {};

        template<int OFFSET, int LENGTH>
        [[nodiscard]] array<unsigned char, LENGTH> GetBlockBytes() const
        {
            array<unsigned char, LENGTH> block = { };
            std::copy(begin(m_bytes) + OFFSET, begin(m_bytes) + OFFSET + LENGTH, begin(block));
            return block;
        }

        template<int LENGTH>
        static string FormatBytes(array<unsigned char, LENGTH> bytes)
        {
            stringstream ss;;

            for (const auto& byte : bytes)
            {
                ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
            }

            return ss.str();
        }

        template<int LENGTH>
        static void Reset(array<unsigned char, LENGTH> bytes)
        {
            for (auto& byte : bytes)
            {
                byte = 0;
            }
        }

        template<int LENGTH>
        static int ParseHex(const string& text, array<unsigned char, LENGTH> &bytes)
        {
            Reset(bytes);

            const auto value = StringUtils::RemoveAny(text, "-");

            const auto length = static_cast<int>(min(value.length() / 2, static_cast<size_t>(LENGTH)));

            for (auto i = 0; i < length; i++)
            {
                const auto byteString = value.substr(i * 2, 2);
                bytes[i] = static_cast<unsigned char>(std::stoul(byteString, nullptr, 16));
            }

            return length;
        }

        [[nodiscard]] array<unsigned char, 4> GetBlock1Bytes() const { return GetBlockBytes<0, 4>(); }
        [[nodiscard]] array<unsigned char, 2> GetBlock2Bytes() const { return GetBlockBytes<4, 2>(); }
        [[nodiscard]] array<unsigned char, 2> GetBlock3Bytes() const { return GetBlockBytes<6, 2>(); }
        [[nodiscard]] array<unsigned char, 2> GetBlock4Bytes() const { return GetBlockBytes<8, 2>(); }
        [[nodiscard]] array<unsigned char, 6> GetBlock5Bytes() const { return GetBlockBytes<10, 6>(); }

        [[nodiscard]] string GetBlock1Digits() const { return FormatBytes(GetBlock1Bytes()); }
        [[nodiscard]] string GetBlock2Digits() const { return FormatBytes(GetBlock2Bytes()); }
        [[nodiscard]] string GetBlock3Digits() const { return FormatBytes(GetBlock3Bytes()); }
        [[nodiscard]] string GetBlock4Digits() const { return FormatBytes(GetBlock4Bytes()); }
        [[nodiscard]] string GetBlock5Digits() const { return FormatBytes(GetBlock5Bytes()); }

    public:
        explicit Guid(array<unsigned char, 16> bytes);
        explicit Guid(const string& guid);

        [[nodiscard]] string str() const;
        [[nodiscard]] string str(GuidFormatType format_type) const;

        static Guid NewGuid();
        static Guid NewGuid(GuidGeneratorType generator_type);
        static Guid Empty();
    };
}
