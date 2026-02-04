#include "stdafx.h"
#include "App.h"
#include "../../libs/DNX.Utils/Crypto/Guid.h"
#include <chrono>
#include <iostream>
#include <regex>

using namespace std;
using namespace std::chrono;
using namespace GuidGenerator;

// ReSharper disable CppTooWideScope
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppClangTidyConcurrencyMtUnsafe

string App::FormatGuid(const Crypto::Guid guid, const string& format)
{
    if (format == "n")
        return guid.str(Crypto::GuidFormatType::Digits);
    if (format == "d")
        return guid.str(Crypto::GuidFormatType::Formatted);
    if (format == "b")
        return StringUtils::EnsureStartsAndEndsWith(guid.str(Crypto::GuidFormatType::Formatted), "{", "}");
    if (format == "p")
        return StringUtils::EnsureStartsAndEndsWith(guid.str(Crypto::GuidFormatType::Formatted), "(", ")");
    if (format == "N")
        return StringUtils::ToUpper(App::FormatGuid(guid, "n"));
    if (format == "D")
        return StringUtils::ToUpper(App::FormatGuid(guid, "d"));
    if (format == "B")
        return StringUtils::ToUpper(App::FormatGuid(guid, "b"));
    if (format == "P")
        return StringUtils::ToUpper(App::FormatGuid(guid, "p"));
    return guid.str(Crypto::GuidFormatType::Default);
}

//------------------------------------------------------------------------------
// Execute
void App::Execute(AppArguments& arguments)
{
    const auto count = arguments.GetCount();

    const auto id_width = to_string(count).length();

    for (auto sequence=1; sequence <= count; ++sequence)
    {
        const auto value = Crypto::GuidGenerator::GenerateGuid(arguments.GetGuidGeneratorType());
        const auto guid = Crypto::Guid(value);

        const auto guid_value = FormatGuid(guid, arguments.GetGuidFormat());

        const auto output_format = arguments.GetOutputFormat();
        auto output_value = StringUtils::Replace(output_format, "{guid}",guid_value);
        output_value = StringUtils::Replace(output_value, "{seq}", to_string(sequence));
        output_value = StringUtils::Replace(output_value, "{id}", StringUtils::LPad(to_string(sequence), id_width, '0'));

        cout << output_value << endl;
    }
}
