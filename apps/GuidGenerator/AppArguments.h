#pragma once

#include "stdafx.h"
#include "../../libs/DNX.App/Arguments.h"
#include "../../libs/DNX.Utils/Crypto/GuidGenerator.h"

#include <chrono>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppDeclaratorNeverUsed

using namespace std;
using namespace std::chrono;
using namespace DNX::Utils;

// Sources:
// https://ambreen2006.medium.com/exploring-date-and-time-with-chrono-3a8e9af60f62
// https://stackoverflow.com/questions/10654258/get-millisecond-part-of-time
// https://en.cppreference.com/w/cpp/chrono/system_clock
// https://en.cppreference.com/w/cpp/chrono/time_point
namespace GuidGenerator
{
    class GuidGeneratorTypeTextResolver : public EnumTextResolver<Crypto::GuidGeneratorType>
    {
    public:
        GuidGeneratorTypeTextResolver()
        {
            SetText(Crypto::GuidGeneratorType::OS, "OS");
            SetText(Crypto::GuidGeneratorType::DateTimeOffset, "DateTime");
        }
    };

    //------------------------------------------------------------------------------
    // Arguments
    class AppArguments final : public Arguments
    {
        const string ArgumentNameGenerator    = "generator";
        const string ArgumentNameCount        = "count";
        const string ArgumentNameGuidFormat   = "format";
        const string ArgumentNameOutputFormat = "output";

        GuidGeneratorTypeTextResolver GuidGeneratorTypeTextConverter;

    public:
        AppArguments()
        {
            AddOption(ValueType::ENUM, "g", ArgumentNameGenerator, GuidGeneratorTypeTextConverter.GetText(Crypto::GuidGeneratorType::OS), "The Guid generation method to use", false, 0, GuidGeneratorTypeTextConverter.GetAllText());
            AddOption(ValueType::INT, "c", ArgumentNameCount, "1", "How many Guids to generate", false);
            AddOption(ValueType::STRING, "f", ArgumentNameGuidFormat, "", "The .NET format to use to output the Guid value(s)", false);
            AddOption(ValueType::STRING, "of", ArgumentNameOutputFormat, "{guid}", "The output format to use when writing the value(s)", false);

            AddNote("Guid Format options:");
            AddNote(" N - 00000000000000000000000000000000");
            AddNote(" D - 00000000-0000-0000-0000-000000000000");
            AddNote(" B - {00000000-0000-0000-0000-000000000000}");
            AddNote(" P - (00000000-0000-0000-0000-000000000000)");
            AddNote("(Lower case format specifiers generate values in Lowercase format)");
            AddNote("See also : https://learn.microsoft.com/en-us/dotnet/api/system.guid.tostring");
            //AddNote(" X - {0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}");
            AddNote("");
            AddNote("Output Format options:");
            AddNote(" {guid} - The Guid value");
            AddNote(" {seq}  - The incrementing sequence number of generated values (From: 1)");
            AddNote(" {id}   - {sequence} left padded with zeroes");
        }

        Crypto::GuidGeneratorType GetGuidGeneratorType()
        {
            return GuidGeneratorTypeTextConverter.GetValue(GetArgumentValue(ArgumentNameGenerator));
        }

        int GetCount()
        {
            return ValueConverter::ToInt(GetArgumentValue(ArgumentNameCount));
        }

        string GetGuidFormat()
        {
            return GetArgumentValue(ArgumentNameGuidFormat);
        }

        string GetOutputFormat()
        {
            return GetArgumentValue(ArgumentNameOutputFormat);
        }
    };
}
