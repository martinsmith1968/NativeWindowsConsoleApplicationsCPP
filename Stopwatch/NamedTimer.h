#pragma once

#include <string>
#include <ctime>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace Stopwatch
{
    class NamedTimer
    {
        string m_Name;
        time_t m_Start;
        long m_Elapsed;

    public:
        NamedTimer() = default;
        explicit NamedTimer(const string& name);

        string GetName() const;
        time_t GetStart() const;
        long GetElapsed() const;

        bool IsEmpty() const;

        static NamedTimer Create(const string& definition);

        [[nodiscard]] string ToDefinition() const;
        void FromDefinition(const string& definition);
    };
}
