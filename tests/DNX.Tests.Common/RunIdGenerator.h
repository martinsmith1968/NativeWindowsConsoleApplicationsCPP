#pragma once

#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticPadded

using namespace std;

namespace DNX::Tests::Common
{
    class RunIdGenerator
    {
        tm m_start_datetime;
        string m_run_id;

    public:
        RunIdGenerator();

        [[nodiscard]] tm GetStartDateTime() const;
        [[nodiscard]] string GetRunId() const;
    };
}
